CFLAGS=-std=c99 -Wall -Wextra -pedantic
.PHONY: clean

arcade: arcade.o player.o game.o
	$(CC) $(CFLAGS) -o $@ $^ -lrt -lpthread

arcade.o: arcade.c arcade.h game.h
	$(CC) $(CFLAGS) -c -o $@ arcade.c

player.o: player.c player.h arcade.h game.h
	$(CC) $(CFLAGS) -c -o $@ player.c

game.o: game.c game.h
	$(CC) $(CFLAGS) -c -o $@ game.c

clean:
	rm -rf arcade *.o