#include "eis.h"

void astud(struct student s)
{
	int sd = handle(
		"opening students database [astud]",
		oplock(STUDENTS, O_WRONLY | O_APPEND)
	);
	write(sd, &s, sizeof(s));
	close(sd);
}

void acourse(struct course c)
{
	int cd = handle(
		"opening courses database [acourse]",
		oplock(COURSES, O_WRONLY | O_APPEND)
	);
	
	write(cd, &c, sizeof(c));
	close(cd);
}

void agrade(struct grade g)
{
	int gd = handle(
		"opening grades database [agrade]",
		oplock(GRADES, O_WRONLY | O_APPEND)
	);

	write(gd, &g, sizeof(g));
	close(gd);
}

int fgrade(struct grade g)
{
	int gd = handle(
		"opening grades database [fgrade]",
		oplock(GRADES, O_RDONLY)
	);

	/* Search for given grade in the database. */
	struct grade c;
	while (read(gd, &c, sizeof(c)) > 0) {
		if (g.id == c.id && g.index == c.index) {
			return c.value;
		}
	}

	close(gd);
	return -1;
}

int fgnames(char *sname, char *cname)
{
	bool found; /* Has record been found? */
	
	/* Search for index of given student name. */
	int sd = handle(
		"opening students database [fgnames]",
		oplock(STUDENTS, O_RDONLY)
	);
	
	found = false;
	struct student s;
	while (read(sd, &s, sizeof(s)) > 0) {
		if (strcmp(s.name, sname) == 0) {
			found = true;
			break;
		}
	}
	close(sd);

	if (!found) {
		return -1;
	}

	/* Serch for course id of given course name. */
	int cd = handle(
		"opening courses database [fgnames]",
		oplock(COURSES, O_RDONLY)
	);

	found = false;
	struct course c;
	while (read(cd, &c, sizeof(c)) > 0) {
		if (strcmp(c.name, cname) == 0) {
			found = true;
			break;
		}
	}
	close(cd);

	if (!found) {
		return -1;
	}

	struct grade g;
	g.index = s.index;
	g.id = c.id;

	return fgrade(g);
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
		oplock(GRADES, O_RDONLY | O_CREAT)
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
		oplock(GRADES, O_WRONLY | O_TRUNC)
	);
	lseek(nd, 0, SEEK_SET);

	while (read(gd, &c, sizeof(c)) > 0) {
		write(gd, &c, sizeof(c));
	}

	close(nd);
	close(gd);
}
