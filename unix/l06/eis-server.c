#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>

#include <netinet/in.h>

#include "eis.h"
#include "utils.h"

#define BUFF_LEN 65536
char buff[BUFF_LEN];

#define REQUEST(...) sscanf(buff, __VA_ARGS__)

void prcsreq(int fd)
{
	struct student s;
	struct course c;
	struct grade g;
	int from = -1, to = -1;

	if (
		REQUEST("ADD STUDENT %d %s", &s.index, s.name) == 2
	) {
		astud(s);
		sprintf(buff, "OK\n");
	} else if (
		REQUEST("ADD COURSE %d %s", &c.id, c.name) == 2
	) {
		acourse(c);
		sprintf(buff, "OK\n");
	} else if (
		REQUEST("ADD GRADE %d %d %d", &g.index, &g.id, &g.value) == 3
	) {
		agrade(g);
		sprintf(buff, "OK\n");
	} else if (
		REQUEST("FIND %d %d", &g.index, &g.id) == 2
	) {
		int grade = fgrade(g);
		if (grade > 0) {
			sprintf(buff, "GRADE %d\n", grade);
		} else {
			sprintf(buff, "NONE\n");
		}
	} else if (
		REQUEST("MATCH OFFSET %s %s %d %d", s.name, c.name, &from, &to)
	) {
		foffset(s.name, c.name, fd, from, to);
	} else if (
		REQUEST("MATCH %s %s", s.name, c.name) == 2
	) {
		int grade = fgnames(s.name, c.name);
		if (grade > 0) {
			sprintf(buff, "GRADE %d\n", grade);
		} else {
			sprintf(buff, "NONE\n");
		}
	} else if (
		REQUEST("MODIFY %d %d %d", &g.index, &g.id, &g.value) == 3
	) {
		mgrade(g);
		sprintf(buff, "OK\n");
	} else if (
		REQUEST("REMOVE %d %d", &g.index, &g.id)
	) {
		rgrade(g);
		sprintf(buff, "OK\n");
	} else {
		printf("Invalid request.\n");
	}

	/* Send reply for request. */
	if (from == -1 && to == -1) {
		write(fd, buff, strlen(buff));
	}
}

int main()
{
	int sock = handle(
		"creating server socket",
		socket(AF_INET, SOCK_STREAM, 0)
	);

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = 0;

	socklen_t alen = sizeof(addr);
	handle(
		"bind socket",
		bind(sock, (struct sockaddr *)&addr, alen)
	);
	handle(
		"get socket name",
		getsockname(sock, (struct sockaddr *)&addr, &alen)
	);
	handle(
		"make socket to a passive mode",
		listen(sock, SOMAXCONN)
	);

	printf("Server listening on port %d.\n", ntohs(addr.sin_port));

	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(sock, &fds);

	while (1) {
		fd_set scpy = fds;
		printf("Waiting...\n");

		handle(
			"server select",
			select(
				FD_SETSIZE,
				&scpy, (fd_set *)0, (fd_set *)0,
				(struct timeval *)0
			)
		);

		for (int fd = 0; fd < FD_SETSIZE; fd++) {
			if (!FD_ISSET(fd, &scpy)) {
				continue;
			}

			if (fd == sock) {
				/* New connection. */
				struct sockaddr_in caddr;
				socklen_t calen = sizeof(caddr);

				int csock = handle(
					"accepting client connection",
					accept(sock, (struct sockaddr *)&caddr, &calen)
				);
				FD_SET(csock, &fds);

				printf("New client: %d.\n", csock);
			} else {
				size_t len = read(fd, buff, BUFF_LEN);

				if (len == 0) {
					/* Client disconnected. */
					close(fd);
					FD_CLR(fd, &fds);

					printf("Connection closed: %d.\n", fd);
				} else {
					/* New message. */
					buff[len] = 0;
					printf("Request from %d:\n%s\n", fd, buff);
					prcsreq(fd);
				}
			}
		}
	}

	printf("Server listening on port: %d\n", ntohs(addr.sin_port));

	return 0;
}
