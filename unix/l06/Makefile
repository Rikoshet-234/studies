CC=gcc
CFLAGS=-Wall -pedantic -std=c99

eis-batch: eis.o utils.o eis-batch.c
	$(CC) $(CFLAGS) -o $@ $^

eis-server: eis.o utils.o eis-server.c
	$(CC) $(CFLAGS) -o $@ $^

eis-client: eis.o utils.o eis-client.c
	$(CC) $(CFLAGS) -o $@ $^ -lcurses -lnsl

eis.o: eis.c
	$(CC) $(CFLAGS) -c -o $@ $<

utils.o: utils.c
	$(CC) $(CFLAGS) -c -o $@ $<

reset:
	rm -rf db/
	mkdir db
	touch db/students.db db/courses.db db/grades.db

samples: reset eis-batch
	./eis-batch -s 247952 Hanuszczak
	./eis-batch -s 997997 Popek
	./eis-batch -s 112233 Abacki
	./eis-batch -s 332211 Zabacki
	./eis-batch -s 151515 Foobar
	
	./eis-batch -k 1 Programowanie
	./eis-batch -k 2 Unix
	./eis-batch -k 3 Dyskretna
	./eis-batch -k 4 Algorytmy
	./eis-batch -k 5 Sieci

	./eis-batch -a 247952 1 5
	./eis-batch -a 247952 2 5
	./eis-batch -a 997997 1 2
	./eis-batch -a 997997 3 2
	./eis-batch -a 247952 4 5
	./eis-batch -a 112233 5 4

	./eis-batch -s 123457 Babacki
	./eis-batch -s 123476 Cabacki
	./eis-batch -s 123233 Zabacki
	./eis-batch -s 742141 Agsahad
	./eis-batch -s 281823 Ashdsah
	./eis-batch -s 123234 Ghsjaas
	./eis-batch -s 214532 Hjashfd

	./eis-batch -a 123457 4 2
	./eis-batch -a 123457 5 3
	./eis-batch -a 123457 2 4
	./eis-batch -a 123457 3 5

	./eis-batch -a 123476 4 2
	./eis-batch -a 123476 5 3
	./eis-batch -a 123476 2 4
	./eis-batch -a 123476 3 5

	./eis-batch -a 123233 4 2
	./eis-batch -a 123233 5 3
	./eis-batch -a 123233 2 4
	./eis-batch -a 123233 3 5

	./eis-batch -a 742141 4 2
	./eis-batch -a 742141 5 3
	./eis-batch -a 742141 2 4
	./eis-batch -a 742141 3 5

	./eis-batch -a 281823 4 2
	./eis-batch -a 281823 5 3
	./eis-batch -a 281823 2 4
	./eis-batch -a 281823 3 5
	
	./eis-batch -a 123234 4 2
	./eis-batch -a 123234 5 3
	./eis-batch -a 123234 2 4
	./eis-batch -a 123234 3 5
	
	./eis-batch -a 214532 4 2
	./eis-batch -a 214532 5 3
	./eis-batch -a 214532 2 4
	./eis-batch -a 214532 3 5

lib: eis.o utils.o
	ar -r eis.a eis.o utils.o
	ranlib eis.a

clean:
	rm -rf *.o
	rm -rf eis.a
	rm -rf eis-batch eis-client eis-server
	rm -rf db/
