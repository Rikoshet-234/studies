/*
 * SYSTEMY OPERACYJNE
 * Prosty system plików.
 *
 * autor: Łukasz Hanuszczak
 * indeks: 247952
 */

#include "sefs.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>


static void sefs_update_space(sefs_t *sefs, size_t index, bool value);
static void sefs_seek_block(sefs_t *sefs, size_t index);
static void sefs_read_block(sefs_t *sefs, size_t index, sefs_block_t *block);
static void sefs_write_block(sefs_t *sefs, size_t index, sefs_block_t *block);

static size_t sefs_index_block_create(sefs_t *sefs);
static size_t sefs_control_block_create(sefs_t *sefs, const char *filepath);

static size_t sefs_file_lookup(sefs_t *sefs, const char *filepath);
static size_t sefs_lookup_free(sefs_t *sefs);
static size_t sefs_block_index(sefs_file_t *file, size_t id);

static size_t sefs_file_extend(sefs_file_t *file);


void sefs_init(const char *filepath, size_t size)
{
	FILE *disk = fopen(filepath, "wb+");

	/* Save disk size. */
	fwrite(&size, sizeof(size), 1, disk);
	
	/* Save free space vector (which should be zeros at this moment). */
	bool *space = malloc(SEFS_BLOCK_COUNT(size));
	memset(space, 0, SEFS_BLOCK_COUNT(size));
	fwrite(space, 1, SEFS_BLOCK_COUNT(size), disk);
	free(space);

	/* Save blocks data (just 0). */
	void *data = malloc(sizeof(sefs_block_t));
	for (size_t block = 0; block < SEFS_BLOCK_COUNT(size); block++) {
		fwrite(data, sizeof(sefs_block_t), 1, disk);
	}
	free(data);

	fclose(disk);
}


sefs_t *sefs_mount(const char *filepath)
{
	FILE *disk = fopen(filepath, "rb+");
	if (!disk) {
		return NULL;
	}

	sefs_t *sefs = malloc(sizeof(sefs_t));
	sefs->disk = disk;
	fread(&(sefs->size), sizeof(sefs->size), 1, sefs->disk);
	sefs->space = malloc(sefs->size);
	fread(sefs->space, 1, sefs->size, sefs->disk);

	return sefs;
}


void sefs_unmount(sefs_t *sefs)
{
	free(sefs->space);
	fclose(sefs->disk);
	free(sefs);
}


size_t sefs_space_avaliable(sefs_t *sefs)
{
	size_t blocks = 0;
	for (size_t i = 0; i < SEFS_BLOCK_COUNT(sefs->size); i++) {
		if (!sefs->space[i]) {
			blocks++;
		}
	}
	return blocks * sizeof(sefs_block_t);
}


static void sefs_update_space(sefs_t *sefs, size_t index, bool value)
{
	/* We have to update value both in memory and on disk. */
	sefs->space[index] = value;

	fseek(sefs->disk, sizeof(FILE *) + sizeof(size_t), SEEK_SET);
	fwrite(sefs->space, 1, SEFS_BLOCK_COUNT(sefs->size), sefs->disk);
}


static void sefs_seek_block(sefs_t *sefs, size_t index)
{
	const size_t OFFSET = sizeof(sefs->size) + sefs->size;
	fseek(sefs->disk, OFFSET + index * sizeof(sefs_block_t), SEEK_SET);
}


static void sefs_read_block(sefs_t *sefs, size_t index, sefs_block_t *block)
{
	sefs_seek_block(sefs, index);
	fread(block, sizeof(sefs_block_t), 1, sefs->disk);
}


static void sefs_write_block(sefs_t *sefs, size_t index, sefs_block_t *block)
{
	/* If we are writing to free space, we have to update free space vector. */
	if (!(sefs->space[index])) {
		sefs_update_space(sefs, index, true);
	}

	sefs_seek_block(sefs, index);
	fwrite(block, sizeof(sefs_block_t), 1, sefs->disk);
}


static size_t sefs_lookup_free(sefs_t *sefs)
{
	for (size_t i = 0; i < SEFS_BLOCK_COUNT(sefs->size); i++) {
		if (!(sefs->space[i])) {
			return i;
		}
	}
	return SEFS_BLOCK_NULL;
}


static size_t sefs_file_lookup(sefs_t *sefs, const char *filepath)
{
	sefs_block_t block;

	for (size_t i = 0; i < SEFS_BLOCK_COUNT(sefs->size); i++) {
		/* We are only interested in non-empty blocks. */
		if (!(sefs->space[i])) {
			continue;
		}

		sefs_read_block(sefs, i, &block);
		if (block.tag == SEFS_CONTROL_BLOCK &&
		    strcmp(block.filepath, filepath) == 0)
		{
			return i;
		}
	}

	return SEFS_BLOCK_NULL;
}


static size_t sefs_control_block_create(sefs_t *sefs, const char *filepath)
{
	size_t index = sefs_lookup_free(sefs);
	assert(index != SEFS_BLOCK_NULL);

	sefs_block_t control_block;
	strcpy(control_block.filepath, filepath);
	control_block.tag = SEFS_CONTROL_BLOCK;
	control_block.index_block = SEFS_BLOCK_NULL;
	sefs_write_block(sefs, index, &control_block);
	return index;
}


sefs_file_t *sefs_open(sefs_t *sefs, const char *filepath)
{
	/* If control block for given file is not found, we have to create it. */
	size_t index = sefs_file_lookup(sefs, filepath);
	if (index == SEFS_BLOCK_NULL) {
		index = sefs_control_block_create(sefs, filepath);
	}
	
	sefs_file_t *file = malloc(sizeof(sefs_file_t));
	file->sefs = sefs;
	file->cursor = 0;
	file->control_block = index;
	return file;
}


void sefs_close(sefs_file_t *file)
{
	free(file);
}


void sefs_create(sefs_t *sefs, const char *filepath)
{
	sefs_remove(sefs, filepath); /* Remove old one just in case. */
	sefs_control_block_create(sefs, filepath);
}


void sefs_remove(sefs_t *sefs, const char *filepath)
{
	size_t index = sefs_file_lookup(sefs, filepath);
	if (index == SEFS_BLOCK_NULL) {
		/* Nothing to delete. */
		return;
	}

	sefs_block_t block;
	sefs_read_block(sefs, index, &block);
	assert(block.tag == SEFS_CONTROL_BLOCK);

	sefs_update_space(sefs, index, false);

	for (index = block.index_block;
	     index != SEFS_BLOCK_NULL;
	     index = block.next_index_block)
	{
		sefs_read_block(sefs, index, &block);
		for (size_t i = 0; i < SEFS_INDEX_BLOCK_CAPACITY; i++) {
			fprintf(stderr, "<%zu>", block.indices[i]);
			fprintf(stderr, "(%zu)", SEFS_BLOCK_NULL);
			sefs_update_space(sefs, block.indices[i], false);
		}
	}
}


static size_t sefs_block_index(sefs_file_t *file, size_t id)
{
	sefs_block_t block;
	sefs_read_block(file->sefs, file->control_block, &block);

	/* Traverse index block list until we got the one with given index. */
	size_t index_block_index = block.index_block;
	while (index_block_index != SEFS_BLOCK_NULL) {
		sefs_read_block(file->sefs, index_block_index, &block);
		if (id < SEFS_INDEX_BLOCK_CAPACITY) {
			return block.indices[id];
		} else {
			id -= SEFS_INDEX_BLOCK_CAPACITY;
			index_block_index = block.next_index_block;
		}
	}

	return SEFS_BLOCK_NULL;
}


static size_t sefs_index_block_create(sefs_t *sefs)
{
	size_t index = sefs_lookup_free(sefs);
	if (index == SEFS_BLOCK_NULL) {
		return SEFS_BLOCK_NULL;
	}

	sefs_block_t index_block;
	index_block.tag = SEFS_INDEX_BLOCK;
	index_block.next_index_block = SEFS_BLOCK_NULL;
	for (size_t i = 0; i < SEFS_INDEX_BLOCK_CAPACITY; i++) {
		index_block.indices[i] = SEFS_BLOCK_NULL;
	}

	sefs_write_block(sefs, index, &index_block);
	return index;
}


static size_t sefs_file_extend(sefs_file_t *file)
{
	sefs_block_t block;
	sefs_read_block(file->sefs, file->control_block, &block);

	if (block.index_block == SEFS_BLOCK_NULL) {
		block.index_block = sefs_index_block_create(file->sefs);
		sefs_write_block(file->sefs, file->control_block, &block);
	}

	size_t index_block_index = block.index_block;
	while (index_block_index != SEFS_BLOCK_NULL) {
		sefs_read_block(file->sefs, index_block_index, &block);

		/* Find first index pointing to null block. */
		size_t i = 0;
		while (i < SEFS_INDEX_BLOCK_CAPACITY &&
		       block.indices[i] != SEFS_BLOCK_NULL)
		{
			i++;
		}

		if (i == SEFS_INDEX_BLOCK_CAPACITY) {
			size_t *next = &block.next_index_block;
			/* Well, that block is full, we have to go to another one. */
			if (*next == SEFS_BLOCK_NULL) {
				*next = sefs_index_block_create(file->sefs);
				sefs_write_block(file->sefs, index_block_index, &block);
			}
			index_block_index = *next;
		} else {
			size_t index = sefs_lookup_free(file->sefs);
			block.indices[i] = index;
			sefs_write_block(file->sefs, index_block_index, &block);
			sefs_write_block(file->sefs, index, &block);
			return index;
		}
	}

	return SEFS_BLOCK_NULL;
}


void sefs_write(sefs_file_t *file, size_t size, const void *buffer)
{
	sefs_block_t block;
	size_t index = sefs_block_index(file, file->cursor / SEFS_BLOCK_CAPACITY);
	if (index == SEFS_BLOCK_NULL) {
		index = sefs_file_extend(file);
	}

	assert(index != SEFS_BLOCK_NULL);
	
	const size_t OFFSET = file->cursor % SEFS_BLOCK_CAPACITY;
	const size_t AVALIABLE = SEFS_BLOCK_CAPACITY - OFFSET;
	if (size <= AVALIABLE) {
		memcpy(block.data + OFFSET, buffer, size);
		sefs_write_block(file->sefs, index, &block);
		file->cursor += size;
	} else {
		memcpy(block.data + OFFSET, buffer, AVALIABLE);
		sefs_write_block(file->sefs, index, &block);
		file->cursor += AVALIABLE;
		sefs_write(file, AVALIABLE, (uint8_t *)buffer + AVALIABLE);
	}
}


void sefs_read(sefs_file_t *file, size_t size, void *buffer)
{
	sefs_block_t block;
	size_t index = sefs_block_index(file, file->cursor / SEFS_BLOCK_CAPACITY);
	sefs_read_block(file->sefs, index, &block);

	assert(index != SEFS_BLOCK_NULL);

	const size_t OFFSET = file->cursor % SEFS_BLOCK_CAPACITY;
	const size_t AVALIABLE = SEFS_BLOCK_CAPACITY - OFFSET;
	if (size <= AVALIABLE) {
		memcpy(buffer, block.data + OFFSET, size);
		file->cursor += size;
	} else {
		memcpy(buffer, block.data + OFFSET, AVALIABLE);
		file->cursor += AVALIABLE;
		sefs_read(file, AVALIABLE, (uint8_t *)buffer + AVALIABLE);
	}
}


void sefs_seek(sefs_file_t *file, size_t position)
{
	file->cursor = position;
}


void sefs_rewind(sefs_file_t *file)
{
	sefs_seek(file, 0);
}
