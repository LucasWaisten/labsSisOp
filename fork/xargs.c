#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>

#ifndef NARGS
#define NARGS 4
#endif


void proceso_fork(char *comandos[]);

void
proceso_fork(char *comandos[])
{
	pid_t pidC;
	pidC = fork();
	if (pidC < 0) {
		perror("Error en el fork");
	}

	if (pidC > 0) {
		wait(NULL);
	} else if (pidC == 0) {
		execvp(comandos[0], comandos);
	}
}

int
main(int argc, char *argv[])
{
	char *line = NULL;
	size_t len = 0;

	char *comandos[NARGS + 2] = { argv[1], NULL, NULL, NULL, NULL, NULL };

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <file>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int iteracion = 1;

	while (getline(&line, &len, stdin) != -1) {
		line[strlen(line) - 1] = '\0';
		comandos[iteracion] = strdup(line);
		if (iteracion == 4) {
			proceso_fork(comandos);
			iteracion = 0;
		}

		iteracion++;
	}

	proceso_fork(comandos);

	free(line);
	exit(EXIT_SUCCESS);

	return 0;
}