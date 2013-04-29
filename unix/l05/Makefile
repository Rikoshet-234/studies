CC=gcc
CLAGS=-Wall -pedantic -c99

prog: eis.o utils.o main.c
	$(CC) $(CFLAGS) -o $@ $^

eis.o: eis.c
	$(CC) $(CFLAGS) -c -o $@ $<

utils.o: utils.c
	$(CC) $(CFLAGS) -c -o $@ $<

reset:
	rm -rf db/*
	touch db/students.db db/courses.db db/grades.db

samples: reset prog
	./prog -s 247952 Hanuszczak
	./prog -s 997997 Popek
	./prog -s 112233 Abacki
	./prog -s 332211 Zabacki
	./prog -s 151515 Foobar
	
	./prog -k 1 Programowanie
	./prog -k 2 Unix
	./prog -k 3 Dyskretna
	./prog -k 4 Algorytmy
	./prog -k 5 Sieci

	./prog -a 247952 1 5
	./prog -a 247952 2 5
	./prog -a 997997 1 2
	./prog -a 997997 3 2
	./prog -a 247952 4 5
	./prog -a 112233 5 4

lib: eis.o utils.o
	ar -r eis.a eis.o utils.o
	ranlib eis.a

clean:
	rm -rf *.o
	rm -rf prog
	rm -rf eis.a
	rm -rf eis
