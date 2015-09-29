CC=clang
CFLAGS=-std=c11 -Wall -Wextra -pedantic -O2
LIBS=-lm `gimptool-2.0 --libs`

.PHONY: build install clean

build: distortion

install: build
	gimptool-2.0 --install-bin distortion

clean:
	rm -rf distortion *.o

distortion: main.c texture.o wrapper.o
	$(CC) $(CFLAGS) `gimptool-2.0 --cflags` -o $@ $^ $(LIBS)

texture.o: ../common/texture.c ../common/texture.h ../common/utility.h
	$(CC) $(CFLAGS) -o $@ -c $<

wrapper.o: ../common/wrapper.c ../common/wrapper.h ../common/utility.h
	$(CC) $(CFLAGS) `gimptool-2.0 --cflags` -o $@ -c $<
