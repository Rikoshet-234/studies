#ifndef __EIS_H
#define __EIS_H

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>

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
void mgrade(struct grade g);
void rgrade(struct grade g);

#endif