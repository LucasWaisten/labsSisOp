#include "util.h"
#include <signal.h>
#include <time.h>

// PID HIJO
// Es global para poder pasarselo al handler
int child_pid = 0;

// HANDLER PARA TERMINAR PROCESO HIJO
void
handle_killChild(int sig)
{
	kill(child_pid, SIGKILL);
}

int
main(int argc, char *argv[])
{
	// Compruebo si los parametro son correctos
	if (argc != 3) {
		errno = EINVAL;
		perror("Error en los parametros, pruebe con: timeout <n> "
		       "<comando> \n");
		_exit(-1);
	}

	// Defino como terminar con el proceso hijo
	// Ligamos el handler que termina al proceso hijo a la senal SIGUSR1
	// No hay problema en usar esta senal, esta para ser usada por los usuarios.
	struct sigaction killChild;
	killChild.sa_handler = &handle_killChild;
	sigaction(SIGUSR1, &killChild, NULL);

	// Creacion de evento
	// El evento manda una senal SIGUSR1
	// Esta senal esta definida para matar al proceso hijo
	struct sigevent event;
	event.sigev_notify = SIGEV_SIGNAL;
	event.sigev_signo = SIGUSR1;

	// Creacion de objeto de tiempo
	// Setear duracion
	struct itimerspec itime;
	itime.it_value.tv_sec = atoi(argv[1]);
	itime.it_value.tv_nsec = 0;
	// Setear repeticion - 0. Indica que no se repite
	itime.it_interval.tv_sec = 0;
	itime.it_interval.tv_nsec = 0;

	// Creacion del timer
	timer_t timerId;
	timer_create(CLOCK_REALTIME, &event, &timerId);

	// Inicio el timer
	timer_settime(timerId, 0, &itime, NULL);

	// Hago un fork par aque el proceso hijo ejecute el comando
	child_pid = _fork();
	if (!child_pid)
		execvp(argv[2], argv + 2);
	// El padre espera a que el hijo termine ( incluido haberlo terminado )
	wait(NULL);

	_exit(0);
}