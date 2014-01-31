/*
 * SYSTEMY OPERACYJNE
 * Synchronizacja precesów przy użyciu semaforów.
 *
 * autor: Łukasz Hanuszczak
 * indeks: 247952
 */

#ifndef FILE_PLAYER_H
#define FILE_PLAYER_H


#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "game.h"
#include "arcade.h"


typedef uint8_t schedule_t; /* Harmonogram gracza to 8-bitowa maska. */

#define ON_SCHEDULE(game) (1 << game)


typedef struct {
	int coins; /* Aktualna ilość monet w portfelu gracza. */
	int tokens; /* Aktualna ilość żetonów które gracz posiada. */
	schedule_t schedule; /* Jakie gry gracz musi jeszcze dziś wygrać? */
} player_t;


void player_enter(player_t *player);
void player_exit(player_t *player);
void player_routine(player_t *player);
void player_exchange(player_t *player);
void player_play(player_t *player, game_t game);
bool player_done(player_t *player);


#endif
