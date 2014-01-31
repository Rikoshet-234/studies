/*
 * SYSTEMY OPERACYJNE
 * Synchronizacja precesów przy użyciu semaforów.
 *
 * autor: Łukasz Hanuszczak
 * indeks: 247952
 */

#include "game.h"


const char *game_to_str(game_t game)
{
	switch (game) {
	case MORTAL_KOMBAT: return "Mortal Kombat";
	case SPACE_INVADERS: return "Space Invaders";
	case PAC_MAN: return "Pac-man";
	case ASTEROIRDS: return "Asteroids";
	case SOUL_CALIBUR: return "Soul Calibur";
	case TEKKEN: return "Tekken";
	case HOUSE_OF_THE_DEAD: return "House of the Dead";
	case METAL_SLUG: return "Metal Slug";
	default: return "<unknown>";
	}
}
