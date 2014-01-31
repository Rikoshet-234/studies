/*
 * SYSTEMY OPERACYJNE
 * Synchronizacja precesów przy użyciu semaforów.
 *
 * autor: Łukasz Hanuszczak
 * indeks: 247952
 */

#ifndef FILE_GAME_H
#define FILE_GAME_H


typedef enum {
	MORTAL_KOMBAT,
	SPACE_INVADERS,
	PAC_MAN,
	ASTEROIRDS,
	SOUL_CALIBUR,
	TEKKEN,
	HOUSE_OF_THE_DEAD,
	METAL_SLUG,

	GAMES_COUNT
} game_t;


const char *game_to_str(game_t game);


#endif
