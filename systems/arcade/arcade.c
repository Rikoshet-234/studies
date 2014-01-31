/*
 * SYSTEMY OPERACYJNE
 * Synchronizacja precesów przy użyciu semaforów.
 *
 * autor: Łukasz Hanuszczak
 * indeks: 247952
 */

#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>

#include "game.h"
#include "arcade.h"
#include "player.h"


/* Rezerwuje wspólną pamięć, wygodny alias funkcji `mmap`. */
static void *shmalloc(size_t length);


sem_t *machine_sem;
sem_t *counter_sem;
sem_t *coin_sem;
sem_t *token_sem;
sem_t *register_sem;

int *tray_coins;
int *tray_tokens;
int *clients_left;


static void *shmalloc(size_t length)
{
	return mmap(
		NULL, length,
		PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS,
		-1, 0
	);
}


bool owner_done(void)
{
	return *clients_left == 0;
}


void owner_enter(int clients)
{
	printf("The owner enters his arcade.\n");

	printf("The owner launches machines and prepares the counter.\n");
	machine_sem = shmalloc(sizeof(sem_t) * GAMES_COUNT);
	for (game_t game = 0; game < GAMES_COUNT; game++) {
		sem_init(&machine_sem[game], true, 1);
	}

	printf("The owner prepares the counter and tray for coins.\n");
	counter_sem = shmalloc(sizeof(sem_t));
	sem_init(counter_sem, true, 1);
	coin_sem = shmalloc(sizeof(sem_t));
	sem_init(coin_sem, true, 0);
	token_sem = shmalloc(sizeof(sem_t));
	sem_init(token_sem, true, 0);
	register_sem = shmalloc(sizeof(sem_t));
	sem_init(register_sem, true, 1);

	tray_coins = shmalloc(sizeof(*tray_coins));
	*tray_coins = 0;

	tray_tokens = shmalloc(sizeof(*tray_tokens));
	*tray_tokens = 0;

	clients_left = shmalloc(sizeof(*clients_left));
	*clients_left = clients;
}


void owner_exit(void)
{
	printf("The owner shuts all machines, closes the counter and leaves.\n");

	for (game_t game = 0; game < GAMES_COUNT; game++) {
		sem_destroy(&machine_sem[game]);
	}
	munmap(machine_sem, sizeof(sem_t) * GAMES_COUNT);

	sem_destroy(counter_sem);
	munmap(counter_sem, sizeof(sem_t));
	sem_destroy(coin_sem);
	munmap(coin_sem, sizeof(sem_t));
	sem_destroy(token_sem);
	munmap(token_sem, sizeof(sem_t));
	sem_destroy(register_sem);
	munmap(register_sem, sizeof(sem_t));

	munmap(tray_coins, sizeof(*tray_coins));
	munmap(tray_tokens, sizeof(*tray_tokens));
	munmap(clients_left, sizeof(*clients_left));
}


void owner_routine(void)
{
	while (!owner_done()) {
		owner_exchange();
	}
}


void owner_exchange(void)
{
	sem_wait(coin_sem);

	if (*tray_coins > 0) {
		printf("The owner exchanges %d coins for tokens.\n", *tray_coins);
		*tray_tokens += *tray_coins;
		*tray_coins = 0;
	}

	sem_post(token_sem);	
}


int main(int argc, char **argv)
{
	/*
	 * Wczytywanie ilości klientów które mają odwiedzić salon zadana jako
	 * parametry wywołania programu. Jeżeli ilość nie zaostanie podana,
	 * domyślną wartością jest 2.
	 */
	int clients;
	if (argc > 2) {
		fprintf(stderr, "Too many arguments!");
		return 1;
	} else if (argc == 1) {
		clients = 5;
	} else if (!(sscanf(argv[1], "%d", &clients) == 1)) {
		fprintf(stderr, "Expected number of players, got: \"%s\".\n", argv[1]);
		return 2;
	}

	/*
	 * Właściciel wchodzi i otwiera salon, który następnie zaczyna się
	 * wypełniać spragnionymi wrażeń graczami.
	 */
	owner_enter(clients);

	pid_t pid = 1;
	for (int i = 0; pid != 0 && i < clients; i++) {
		pid = fork();
	}

	if (pid == 0) {
		player_t player;
		player_enter(&player);
		player_routine(&player);
		player_exit(&player);
	} else {
		owner_routine();
		owner_exit();
	}

	return 0;
}
