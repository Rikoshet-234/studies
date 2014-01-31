/*
 * SYSTEMY OPERACYJNE
 * Synchronizacja precesów przy użyciu semaforów.
 *
 * autor: Łukasz Hanuszczak
 * indeks: 247952
 */

#ifndef FILE_ARCADE_H
#define FILE_ARCADE_H


#include <unistd.h>
#include <semaphore.h>


extern sem_t *machine_sem; /* Automaty do gier. */
extern sem_t *counter_sem; /* Kontuar do sprzedaży żetonów. */
extern sem_t *coin_sem; /* Monety. */
extern sem_t *token_sem; /* Żetony. */
extern sem_t *register_sem; /* Rejestr klientów (do liczenia gości). */

extern int *tray_coins; /* Ilość monet na tacy. */
extern int *tray_tokens; /* Ilość żetonów na tacy. */

extern int *clients_left; /* Ilość klientów pozostałych w salonie. */


void owner_enter(int clients);
void owner_exit(void);
void owner_routine(void);
void owner_exchange(void);
bool owner_done(void);


#endif
