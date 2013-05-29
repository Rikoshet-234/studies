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
		open(GRADES, O_RDONLY)
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

int fgnames(char *spatt, char *cpatt)
{
	bool found; /* Has record been found? */
	
	/* Search for index of given student name. */
	int sd = handle(
		"opening students database [fgnames]",
		open(STUDENTS, O_RDONLY)
	);
	
	found = false;
	struct student s;
	while (read(sd, &s, sizeof(s)) > 0) {
		if (match(s.name, spatt)) {
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
		open(COURSES, O_RDONLY)
	);

	found = false;
	struct course c;
	while (read(cd, &c, sizeof(c)) > 0) {
		if (match(c.name, cpatt)) {
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

#define BUFF_LEN 100
static char buff[BUFF_LEN];

void foffset(char *spatt, char *cpatt, int fd, int from, size_t count)
{
	int sd = handle(
		"opening students database [fgnames]",
		open(STUDENTS, O_RDONLY)
	);
	
	/* Search for students with given name. */
	struct student s[BUFF_LEN];
	size_t studs = 0;
	while (read(sd, s + studs, sizeof(struct student)) > 0) {
		if (match(s[studs].name, spatt)) {
			studs++;
		}
	}
	close(sd);

	int cd = handle(
		"opening courses database [fgnames]",
		open(COURSES, O_RDONLY)
	);

	/* Serch for courses with given name. */
	struct course c[BUFF_LEN];
	size_t courses = 0;
	while (read(cd, c + courses, sizeof(struct course)) > 0) {
		if (match(c[0].name, cpatt)) {
			courses++;
		}
	}
	close(cd);

	int cur = -1;
	buff[0] = 0;
	for (int si = 0; si < studs; si++) {
		for (int ci = 0; ci < courses; ci++) {
			struct grade g;
			g.index = s[si].index;
			g.id = c[ci].id;

			int grade = fgrade(g);
			if (grade >= 0) {
				cur++;
			} else {
				continue;
			}

			if (from <= cur) {
				if (cur < from + count) {
					sprintf(
						buff + strlen(buff),
						"GRADE %d (%s, %s)|", grade, s[si].name, c[ci].name
					);
				} else {
					sprintf(buff + strlen(buff), "MORE");

					/* Brilliant loop breaking without goto. */
					si = studs;
					ci = courses;
					break;
				}
			}
		}
	}

	if (cur == -1) {
		sprintf(buff + strlen(buff), "NONE");
	} else if (from <= cur && cur < from + count) {
		sprintf(buff + strlen(buff), "END");
	}

	write(fd, buff, strlen(buff));
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

	char template[] = "/tmp/fileXXXXXX";
	int nd = handle(
		"creating temporary file",
		mkstemp(template)
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
