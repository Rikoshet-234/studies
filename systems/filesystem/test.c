/*
 * SYSTEMY OPERACYJNE
 * Prosty system plików.
 *
 * autor: Łukasz Hanuszczak
 * indeks: 247952
 */

#include <assert.h>
#include <string.h>

#include "sefs.h"


int main(void)
{
	const size_t SIZE = 1024 * 1024;
	const char *DISKNAME = "disk.dat";

	/* Test filesystem initialization and mounting. */
	sefs_init(DISKNAME, SIZE);

	sefs_t *sefs = sefs_mount(DISKNAME);
	assert(sefs);
	assert(sefs->disk);
	assert(sefs->size == SIZE);
	assert(sefs->space);

	/* Test file allocation and lookup. */
	sefs_file_t *afile = sefs_open(sefs, "afile.xoxo");
	sefs_file_t *bfile = sefs_open(sefs, "bfile.xoxo");
	sefs_file_t *xfile = sefs_open(sefs, "afile.xoxo");
	assert(afile);
	assert(bfile);
	assert(xfile);
	assert(afile->control_block == xfile->control_block);

	/* Test writing and reading small, string data. */
	const char obuffer[] = "this is sample string";
	sefs_write(afile, strlen(obuffer), obuffer);

	char ibuffer[200];
	sefs_read(xfile, strlen(obuffer), ibuffer);
	
	assert(memcmp(obuffer, ibuffer, strlen(obuffer)) == 0);

	sefs_close(afile);
	sefs_close(bfile);
	sefs_close(xfile);

	/* Test writing and reading large, integer data. */
	const int N = 1024;

	sefs_file_t *onumbers = sefs_open(sefs, "numbers.dat");
	for (int i = 0; i < N; i++) {
		int e = i;
		sefs_write(onumbers, sizeof(e), &e);
	}

	sefs_file_t *inumbers = sefs_open(sefs, "numbers.dat");
	for (int i = 0; i < N; i++) {
		int e;
		sefs_read(inumbers, sizeof(e), &e);
		assert(i == e);
	}

	sefs_close(onumbers);
	sefs_close(inumbers);

	/* Test space allocation and release. */
	size_t prea = sefs_space_avaliable(sefs);
	sefs_create(sefs, "a");
	size_t preb = sefs_space_avaliable(sefs);
	sefs_create(sefs, "b");
	size_t postb = sefs_space_avaliable(sefs);
	sefs_remove(sefs, "b");
	size_t posta = sefs_space_avaliable(sefs);
	sefs_remove(sefs, "a");
	size_t final = sefs_space_avaliable(sefs);
	assert(prea > preb);
	assert(preb > postb);
	assert(postb < posta);
	assert(posta < final);

	sefs_unmount(sefs);

	return 0;
}
