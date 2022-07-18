#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

void error(char *message);

void
error(char *message)
{
	perror(message);
	_exit(-1);
	return;
}

int
main(void)
{
	printf("hola soy pid %d \n", getpid());
	/* 	Siglas
	 * FtS = Father to Son
	 * StF = Son to Father
	 */
	/* 	posicion 0 -> read;
	        posicion 1 -> write
	*/
	int FtS[2];
	int StF[2];


	int fts = pipe(FtS);
	int stf = pipe(StF);

	if (fts < 0) {
		error("Error en el primer Pipe");
	}
	printf("\t-primer pipe me devuelve [%d %d] \n", FtS[0], FtS[1]);

	if (stf < 0) {
		error("Error en el segundo Pipe");
	}
	printf("\t-segundo pipe me devuelve [%d %d] \n", StF[0], StF[1]);
	printf("\n");

	pid_t pidC;
	pidC = fork();
	if (pidC < 0) {
		error("Error en el fork");
	}

	int msg_envio = rand();
	int msg_recibido;

	switch (pidC) {
	case 0:  // HIJO
		close(FtS[1]);
		close(StF[0]);
		if (read(FtS[0], &msg_recibido, sizeof(msg_recibido)) != -1) {
			printf("donde fork me devuelve %d \n", pidC);
			printf("\t-getpid me devuelve: %d\n\t-getppid me "
			       "devuelve "
			       "%d\n\t-recibo valor %d via fd=%d\n\t-reenvio "
			       "valor "
			       "en fd= %d y termino \n ",
			       getpid(),
			       getppid(),
			       msg_recibido,
			       FtS[0],
			       StF[1]);
		} else {
			error("Error en read");
		}

		if (write(StF[1], &msg_recibido, sizeof(msg_recibido)) == -1)
			error("Error en el write");

		close(StF[1]);
		close(FtS[0]);
		break;
	default:  // padre
		close(FtS[0]);
		close(StF[1]);

		if (write(FtS[1], &msg_envio, sizeof(msg_envio)) != -1) {
			printf("donde fork me devuelve %d \n", pidC);
			printf("\t-getpid me devuelve %d\n\t-getppid me "
			       "devuelve "
			       "%d\n\t-random me devuelve %d\n\t-envio valor "
			       "%d a "
			       "traves de fd=%d\n",
			       getpid(),
			       getppid(),
			       msg_envio,
			       msg_envio,
			       FtS[1]);
		} else {
			error("Error en write");
		}
		close(FtS[1]);

		if (read(StF[0], &msg_recibido, sizeof(msg_recibido)) != -1) {
			printf("hola de nuevo pid %d\n", getpid());
			printf("-recibi valor %d via fd=%d\n",
			       msg_recibido,
			       StF[0]);
		} else {
			error("Error en read");
		}

		close(StF[0]);
		break;
	}
	return 0;
}