#ifndef __EIS_H
#define __EIS_H

#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>

#include "utils.h"

#define STUDENTS "db/students.db"
#define COURSES "db/courses.db"
#define GRADES "db/grades.db"

struct student {
	int index;
	char name[20];
};

struct course {
	int id;
	char name[20];
};

struct grade {
	int index;
	int id;
	int value;
};

void astud(struct student s);
void acourse(struct course c);
void agrade(struct grade g);
int fgrade(struct grade g);
int fgnames(char *sname, char *cname);
void foffset(char *spatt, char *cpatt, int fd, int from, size_t count);
void mgrade(struct grade g);
void rgrade(struct grade g);

#endif
