#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "eis.h"

int main(int argc, char **argv)
{
	if (argc < 3) {
		printf("Wrong number of argument.\n");
		return 1;
	}

	if (strcmp(argv[1], "-s") == 0) {
		struct student s;
		sscanf(argv[2], "%d", &s.index);
		sscanf(argv[3], "%s", s.name);
		astud(s);
		return 0;
	}

	if (strcmp(argv[1], "-k") == 0) {
		struct course c;
		sscanf(argv[2], "%d", &c.id);
		sscanf(argv[3], "%s", c.name);
		acourse(c);
		return 0;
	}

	if (strcmp(argv[1], "-a") == 0) {
		struct grade g;
		sscanf(argv[2], "%d", &g.index);
		sscanf(argv[3], "%d", &g.id);
		sscanf(argv[4], "%d", &g.value);
		agrade(g);
		return 0;
	}

	if (strcmp(argv[1], "-f") == 0) {
		struct grade g;
		sscanf(argv[2], "%d", &g.index);
		sscanf(argv[3], "%d", &g.id);

		int grade = fgrade(g);
		if (grade < 0) {
			printf("Grade not found.\n");
			return 1;
		}
		printf("%d\n", grade);
		return 0;
	}

	if (strcmp(argv[1], "-m") == 0) {		
		struct grade g;
		sscanf(argv[2], "%d", &g.index);
		sscanf(argv[3], "%d", &g.id);
		sscanf(argv[4], "%d", &g.value);
		mgrade(g);
		return 0;
	}

	if (strcmp(argv[1], "-u") == 0) {
		struct grade g;
		sscanf(argv[2], "%d", &g.index);
		sscanf(argv[3], "%d", &g.id);
		rgrade(g);
		return 0;		
	}

	printf("Incorrect argument: %s\n", argv[1]);
	return 1;
}
