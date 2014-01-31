/*
 * SYSTEMY OPERACYJNE
 * Synchronizacja precesów przy użyciu semaforów.
 *
 * autor: Łukasz Hanuszczak
 * indeks: 247952
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include <unistd.h>
#include <sys/types.h>
#include <semaphore.h>

#include "player.h"


bool player_done(player_t *player)
{
	/* Gracz ma dość gdy wygra zaplanowane gry lub wyczerpie środki. */
	return !player->schedule || (player->tokens == 0 && player->coins == 0);
}


void player_enter(player_t *player)
{
	printf("Player %lu arrives at the arcade.\n", (unsigned long)getpid());

	srand(time(NULL) * getpid());
	player->schedule = rand() % 0xff + 1;
	player->coins = GAMES_COUNT;
	player->tokens = 0;
}


void player_exit(player_t *player)
{
	unsigned long pid = getpid();
	printf("Player %lu exits the arcade with %d coins.\n", pid, player->coins);

	sem_wait(register_sem);
	*clients_left -= 1;
	if (*clients_left == 0) {
		printf("Player %lu is last client and notifies the owner.\n", pid);
		sem_post(coin_sem);
	}
	sem_post(register_sem);
}


void player_routine(player_t *player)
{
	if (player_done(player)) {
		return;
	}

	if (player->tokens == 0) {
		player_exchange(player);
	}

	for (game_t game = 0; player->tokens > 0 && game < GAMES_COUNT; game++) {
		if (player->schedule & ON_SCHEDULE(game)) {
			player_play(player, game);
		}
	}

	player_routine(player);
}


void player_play(player_t *player, game_t game)
{
	assert(player->schedule & ON_SCHEDULE(game));
	assert(player->tokens > 0);

	unsigned long pid = getpid();

	printf("Player %lu waits for the %s machine.\n", pid, game_to_str(game));
	sem_wait(&machine_sem[game]);

	printf("Player %lu inserts a token into %s and ", pid, game_to_str(game));
	player->tokens--;
	if (rand() % 3) {
		player->schedule ^= ON_SCHEDULE(game);
		printf("wins!\n");
	} else {
		printf("loses.\n");
	}

	printf("Player %lu leaves the %s machine.\n", pid, game_to_str(game));
	sem_post(&machine_sem[game]);
}


void player_exchange(player_t *player)
{
	assert(player->coins > 0);

	unsigned long pid = getpid();

	/*
	 * Gracz wymienia tyle monet ile potrzebuje aby wygrać wszystkie
	 * zaplanowane przez siebie gry. Liczba ta musi zostać odpowiednio
	 * przycięta do ilości monet jakie gracz ma jeszcze w portfelu.
	 */
	int count = 0;
	for (game_t game = 0; game < GAMES_COUNT; game++) {
		if (player->schedule & ON_SCHEDULE(game)) {
			count++;
		}
	}
	count = (player->coins < count) ? player->coins : count;

	printf("Player %lu waits before the counter.\n", pid);
	sem_wait(counter_sem);

	printf("Player %lu puts %d coins on tray.\n", pid, count);
	*tray_coins += count;
	player->coins -= count;
	sem_post(coin_sem);
	sem_wait(token_sem);

	printf("Player %lu takes %d tokens and leaves the counter.\n", pid, count);
	player->tokens += *tray_tokens;
	*tray_tokens = 0;
	sem_post(counter_sem);
}
