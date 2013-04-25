#include "eis.h"

void astud(struct student s)
{
	int fd = handle(
		"opening students database [astud]",
		open(STUDENTS, O_WRONLY | O_APPEND)
	);
	handle(
		"create lock on students database",
		flock(fd, F_LOCK, 0)
	);

	write(fd, &s, sizeof(s));
	close(fd);
}

void acourse(struct course c)
{
	int fd = handle(
		"opening courses database [acourse]",
		open(COURSES, O_WRONLY | O_APPEND)
	);
	handle(
		"create lock on courses database",
		flock(fd, F_LOCK, 0)
	);
	
	write(fd, &c, sizeof(c));
	close(fd);
}

void agrade(struct grade g)
{
	int fd = handle(
		"opening grades database [agrade]",
		open(GRADES, O_WRONLY | O_APPEND)
	);
	handle(
		"create lock on grades database",
		flock(fd, F_LOCK, 0)
	);

	write(fd, &g, sizeof(g));
	close(fd);
}

int fgrade(struct grade g)
{
	int fd = handle(
		"opening grades database [fgrade]",
		open(GRADES, O_RDONLY)
	);
	handle(
		"create lock on grades database",
		flock(fd, F_LOCK, 0)
	);

	struct grade c;
	while (read(fd, &c, sizeof(c)) > 0) {
		if (g.id == c.id && g.index == c.index) {
			return c.value;
		}
	}

	close(fd);
	return -1;
}

void mgrade(struct grade g)
{
	rgrade(g);
	agrade(g);
}

void rgrade(struct grade g)
{
	int gd = handle(
		"opening grades database [rgrade]",
		open(GRADES, O_RDONLY | O_CREAT)
	);
	handle(
		"create lock on grades database",
		flock(gd, F_LOCK, 0)
	);

	char template[] = "/tmp/fileXXXXXX";
	int nd = handle(
		"creating temporary file",
		mkostemp(template, O_RDWR | O_TRUNC)
	);

	struct grade c;
	while (read(gd, &c, sizeof(c)) > 0) {
		if (!(g.id == c.id && g.index == c.index)) {
			write(nd, &g, sizeof(g));
		}
	}

	close(gd);

	gd = handle(
		"opening grades database",
		open(GRADES, O_WRONLY | O_TRUNC)
	);
	handle(
		"create lock on grades database",
		flock(gd, F_LOCK, 0)
	);
	lseek(nd, 0, SEEK_SET);

	while (read(gd, &c, sizeof(c)) > 0) {
		write(gd, &c, sizeof(c));
	}

	close(nd);
	close(gd);
}
