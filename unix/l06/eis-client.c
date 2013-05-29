#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>

#include <curses.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "utils.h"

static int dbsock;

void mnu_init();
void mnu_act();

typedef unsigned int uint;

enum menu_opt {
	ADD_STUD,
	ADD_CRSE,
	ADD_GRDE,
	FND_GRDE,
	MCH_GRDE,
	MOD_GRDE,
	REM_GRDE,
	MENU_OPTIONS
};

void mnu_init();
void mnu_act();
void mnu_add();
void mnu_add_stud();
void mnu_add_crse();
void mnu_add_grde();
void mnu_fnd_grde();
void mnu_mtch_grde();
void mnu_rem_grde();
void mnu_mod_grde();

const char *menu_str[MENU_OPTIONS];
const char *menu_msg = "Welcome to EIS. What do you want to do?";

#define BUFF_LEN 65536
char buff[BUFF_LEN];

void mnu_init(int sock)
{
	dbsock = sock;
	menu_str[ADD_STUD] = "add student";
	menu_str[ADD_CRSE] = "add course";
	menu_str[ADD_GRDE] = "add grade";
	menu_str[FND_GRDE] = "find grade";
	menu_str[MCH_GRDE] = "match grade";
	menu_str[MOD_GRDE] = "modify grade";
	menu_str[REM_GRDE] = "remove grade";
}

void mnu_act()
{
	int selected = 0;
	while (true) {
		clear();

		const int y = LINES / 2 - MENU_OPTIONS - 2;
		const int x = COLS / 2 - strlen(menu_msg) / 2;
		move(y, x);
		addstr(menu_msg);

		for (uint i = 0; i < MENU_OPTIONS; i++) {
			if (selected == i) {
				attron(A_REVERSE);
			}

			const int y = LINES / 2 - MENU_OPTIONS + i;
			const int x = COLS / 2 - strlen(menu_str[i]) / 2;
			move(y, x);
			addstr(menu_str[i]);

			if (selected == i) {
				attroff(A_REVERSE);
			}
		}
		refresh();

		int key = getch();
		switch (key) {
		case KEY_UP:
			selected--;
			break;
		case KEY_DOWN:
			selected++;
			break;
		case 'q':
			return;
		case '\n':
			mnu_add(selected);
		}
		
		if (selected < 0) {
			selected = MENU_OPTIONS - 1;
		}
		if (selected >= MENU_OPTIONS) {
			selected = 0;
		}
	}
}

void mnu_add(enum menu_opt opt)
{
	switch (opt) {
	case MCH_GRDE:
		mnu_mtch_grde();
		return;
	case ADD_STUD:
		mnu_add_stud();
		break;
	case ADD_CRSE:
		mnu_add_crse();
		break;
	case ADD_GRDE:
		mnu_add_grde();
		break;
	case FND_GRDE:
		mnu_fnd_grde();
		break;
	case REM_GRDE:
		mnu_rem_grde();
		break;
	case MOD_GRDE:
		mnu_mod_grde();
		break;
	default:
		addstr("Unknown screen.\n");
		getch();
		return;
	}

	/* Send request. */
	write(dbsock, buff, strlen(buff));

	/* Parse output. */
	size_t len = handle(
		"reading from socket",
		read(dbsock, buff, BUFF_LEN)
	);
	buff[len] = 0;

	clear();
	printw("Answer: %s\n", buff);
	printw("Press any key to continue.");
	getch();
}

void mnu_add_stud()
{
	char index[10], name[30];
	clear();

	printw("Student index number: ");
	getstr(index);

	printw("Student name: ");
	getstr(name);

	sprintf(buff, "ADD STUDENT %s %s", index, name);
}

void mnu_add_crse()
{
	char id[10], name[30];
	clear();

	printw("Course id: ");
	getstr(id);

	printw("Course name: ");
	getstr(name);

	sprintf(buff, "ADD COURSE %s %s", id, name);
}

void mnu_add_grde()
{
	char id[10], index[10], value[5];
	clear();

	printw("Student index number: ");
	getstr(index);

	printw("Course id number: ");
	getstr(id);

	printw("Grade: ");
	getstr(value);

	sprintf(buff, "ADD GRADE %s %s %s", index, id, value);
}

void mnu_fnd_grde()
{
	char id[10], index[10];
	clear();

	printw("Student index number: ");
	getstr(index);

	printw("Course id number: ");
	getstr(id);

	sprintf(buff, "FIND %s %s", index, id);
}

void mnu_mtch_grde()
{
	char sn[30], cn[30];
	clear();

	printw("Student name: ");
	getstr(sn);

	printw("Course name: ");
	getstr(cn);

	int cur = 0;
	while (true) {
		/* Send offset request for current page. */
		sprintf(buff, "MATCH OFFSET %s %s %d %d", sn, cn, cur, LINES - 1);
		write(dbsock, buff, strlen(buff));

		/* Recive data from socket. */
		size_t len = handle(
			"reciving data from socket",
			read(dbsock, buff, BUFF_LEN)
		);
		buff[len] = 0;

		/* Show results. */
		clear();
		char *tok = strtok(buff, "|");
		while (tok != NULL) {
			if (strcmp(tok, "NONE") == 0) {
				addstr("Nothing found, hit any key to continue.");
				getch();
				return;
			} else if (strcmp(tok, "END") == 0) {
				addstr("Hit any key to continue.");
				getch();
				return;
			} else if (strcmp(tok, "MORE") == 0) {
				addstr("Press q to quit or key down for more.");
				bool more = false;
				while (!more) {
					int key = getch();
					switch (key) {
					case 'q':
						return;
					case KEY_DOWN:
						more = true;
						break;
					}
				}
			} else {
				addstr(tok);
				addstr("\n");
			}
			tok = strtok(NULL, "|");
		}
		cur += LINES / 2;
	}
}

void mnu_rem_grde()
{
	char id[10], index[10];
	clear();

	printw("Student index number: ");
	getstr(index);

	printw("Course id number: ");
	getstr(id);

	sprintf("REMOVE %s %s", index, id);
}

void mnu_mod_grde()
{
	char id[10], index[10], value[5];
	clear();

	printw("Student index number: ");
	getstr(index);

	printw("Course id number: ");
	getstr(id);

	printw("New value: ");
	getstr(value);

	sprintf("MODIFY %s %s %s", index, id, value);
}

int main(int argc, char **argv)
{
	argv++;
	argc--;

	/* Networking sutff. */
	int sock = handle(
		"creating client socket",
		socket(AF_INET, SOCK_STREAM, 0)
	);

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	memset(&addr, sizeof(addr), 0);
	inet_pton(AF_INET, argv[0], &addr.sin_addr);
	
	int port;
	sscanf(argv[1], "%d", &port);
	addr.sin_port = htons(port);

	socklen_t alen = sizeof(addr);
	handle(
		"connect to a server",
		connect(sock, (struct sockaddr *)&addr, alen)
	);


	/* Curses setup. */
	initscr();
	keypad(stdscr, true);

	/* Show menu. */
	mnu_init(sock);
	mnu_act();

	/* Quit. */
	endwin();
	close(sock);
	return 0;
}
