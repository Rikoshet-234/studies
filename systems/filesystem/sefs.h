/*
 * SYSTEMY OPERACYJNE
 * Prosty system plików.
 *
 * autor: Łukasz Hanuszczak
 * indeks: 247952
 */

#ifndef FILE_SEFS_H
#define FILE_SEFS_H


#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


typedef struct {
	FILE *disk;
	size_t size;
	bool *space;
} sefs_t;


#define SEFS_BLOCK_NULL ((size_t)-1)
#define SEFS_BLOCK_CAPACITY 512
#define SEFS_BLOCK_COUNT(size) \
	((size / SEFS_BLOCK_CAPACITY) + (size % SEFS_BLOCK_CAPACITY == 0 ? 0 : 1))
#define SEFS_FILEPATH_LENGTH 128
#define SEFS_INDEX_BLOCK_CAPACITY \
	((SEFS_BLOCK_CAPACITY - sizeof(size_t)) / sizeof(size_t))
#define SEFS_BLOCK_INDEX(cursor) \
	(cursor / SEFS_INDEX_BLOCK_CAPACITY)


typedef struct {
	enum {
		SEFS_CONTROL_BLOCK,
		SEFS_INDEX_BLOCK,
		SEFS_DATA_BLOCK
	} tag;
	union {
		struct {
			char filepath[SEFS_FILEPATH_LENGTH];
			size_t index_block;
		};
		struct {
			size_t indices[SEFS_INDEX_BLOCK_CAPACITY];
			size_t next_index_block;
		};
		uint8_t data[SEFS_BLOCK_CAPACITY];
	};
} sefs_block_t;


typedef struct {
	sefs_t *sefs;
	size_t cursor;
	size_t control_block;
} sefs_file_t;


/**
 * Inicjuje system plików o zadanym rozmiarze.
 *
 * @param filepath Ścieżka do pliku w którym będzie znajdować się system.
 * @param size Rozmiar systemu plików ("pojemność").
 */
void sefs_init(const char *filepath, size_t size);

/**
 * Montuje system plików.
 *
 * @param filepath Ścieżka do systemu plików.
 * @return Uchwyt montowanego systemu plików (lub NULL w razie niepowodzenia).
 */
sefs_t *sefs_mount(const char *filepath);

/**
 * Odmontowuje zadany system plików.
 *
 * @param sefs Uchwyt systemu plików do odmontowania.
 */
void sefs_unmount(sefs_t *sefs);

/**
 * Przelicza ilość wolnej przestrzeni na dysku.
 *
 * @param sefs Uchwyt systemu plików który chcemy sprawdzić.
 * @return Ilość wolnej przestrzeni w bajtach.
 */
size_t sefs_space_avaliable(sefs_t *sefs);

/**
 * Usuwa wskazany plik z dysku.
 *
 * @param sefs Uchwyt system plików z którego usuwamy.
 * @param filepath Ścieżka do pliku który chcemy usunąć.
 */
void sefs_remove(sefs_t *sefs, const char *filepath);

/**
 * Tworzy pusty plik.
 *
 * @param sefs Uchwyt systemu w którym tworzymy plik.
 * @param filepath Ścieżka do pliku który chcemy utworzyć.
 */
void sefs_create(sefs_t *sefs, const char *filepath);

/**
 * Otwiera plik (analogiczne do funkcji `fopen`). W przypadku braku pliku,
 * zostanie on utworzony.
 *
 * @param sefs Uchwyt systemu plików w którym otwieramy plik.
 * @param filepath Ścieżka do pliku który chcemy utworzyć.
 * @return Uchwyt do otwartego pliku (lub NULL w razie niepowodzenia).
 */
sefs_file_t *sefs_open(sefs_t *sefs, const char *filepath);

/**
 * Zamyka plik (analogiczne do funkcji `fclose`).
 *
 * @param file Uchwyt do pliku który chcemy zamknąć.
 */
void sefs_close(sefs_file_t *file);


/**
 * Zapisuje dane z bufora do pliku (analogiczne do funkcji `fwrite`).
 *
 * @param file Uchwyt do pliku do którego chcemy pisać.
 * @param size Ilość bajtów do zapisania (rozmiar bufora).
 * @param buffer Adres bufora który chcemy zapisać.
 */
void sefs_write(sefs_file_t *file, size_t size, const void *buffer);

/**
 * Czyta dane z pliku do bufora (analogiczne do funkcji `fread`).
 *
 * @param file Uchwyt do pliku z którego chcemy czytać.
 * @param size Ilość bajtów do wczytania.
 * @param buffer Adres bufora do którego chcemy wczytać.
 */
void sefs_read(sefs_file_t *file, size_t size, void *buffer);

/**
 * Przesuwa się w pliku (analogiczne do funkcji `fseek`).
 *
 * @param file Uchwyt do pliku w którym chcemy się przesunąć.
 * @param position Nowa pozycja na którą chcemy się przesunąć.
 */
void sefs_seek(sefs_file_t *file, size_t position);

/**
 * Przesuwa się na początek pliku (analogiczne do funkcji `frewind`).
 *
 * @param file Uchwyt do pliku który chcemy przewinąć.
 */
void sefs_rewind(sefs_file_t *file);


#endif
