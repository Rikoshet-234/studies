CC=clang
CFLAGS=-std=c11 -Wall -Wextra -pedantic -O2
LIBS=-lm `gimptool-2.0 --libs`

.PHONY: build install clean

build: median

install: build
	gimptool-2.0 --install-bin median

clean:
	rm -rf median *.o

median: main.c texture.o wrapper.o median.o
	$(CC) $(CFLAGS) `gimptool-2.0 --cflags` -o $@ $^ $(LIBS)

median.o: median.c median.h
	$(CC) $(CFLAGS) -o $@ -c $<

texture.o: ../common/texture.c ../common/texture.h ../common/utility.h
	$(CC) $(CFLAGS) -o $@ -c $<

wrapper.o: ../common/wrapper.c ../common/wrapper.h ../common/utility.h
	$(CC) $(CFLAGS) `gimptool-2.0 --cflags` -o $@ -c $<
