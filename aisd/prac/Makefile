CC=clang++
CFLAGS=-Wall -pedantic -O2 -std=c++11

all: 2013_a 2013_b 2013_c

%: %.cpp
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f 2013_a 2013_b 2013_c
