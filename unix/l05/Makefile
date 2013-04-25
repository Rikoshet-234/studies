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

lib: eis.o utils.o
	ar -r eis.a eis.o utils.o
	ranlib eis.a

clean:
	rm -rf *.o
	rm -rf prog
	rm -rf eis.a
	rm -rf eis
