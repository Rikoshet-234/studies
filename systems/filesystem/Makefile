.PHONY: clean

CC=clang
CFLAGS=-Wall -Wextra -pedantic -std=c11


test: sefs.o test.c
	$(CC) $(CFLAGS) -o $@ sefs.o test.c

sefs.o: sefs.h sefs.c
	$(CC) $(CFLAGS) -c -o $@ sefs.c

clean:
	rm -rf *.o test