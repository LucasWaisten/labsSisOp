#define _GNU_SOURCE
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <sys/user.h>
#include <sys/ptrace.h>
#include <linux/ptrace.h>
#include <syscall.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdlib.h>

#include "syscallent.h"

#define RESET "\x1B[0m"
#define BLU "\x1B[34m"

#define L_ERROR 2
#define L_WARNING 1
#define L_LOG 0


void print_info(struct ptrace_syscall_info sys_info, struct user_regs_struct regs);

void log_(unsigned int type, const char *message);

int
main(int argc, char **argv)
{
	if (argc != 2) {
		perror("Error formato incorrecto, pruebe con './strace <cmd>'");
		exit(-1);
	}

	// Se inicia un trace haciendo un fork, segun lo recomendado en el manual
	pid_t pid = fork();
	// char *filename = argv[1];

	switch (pid) {
	case -1:
		// En caso de error, se reporta el mismo
		log_(L_ERROR, "Error happened while fork()");
		break;
	case 0:
		/*
		El hijo tendra que indidcar al padre que le haga unseguimeinto.
		Para ello se utiliza PTRACE_TRACEME.
		- pid,addr y data debern ser ignorados.
		Acto seguido, se ejecuta un execvp.
	    */
		ptrace(PTRACE_TRACEME, 0, 0, 0);
		execvp(argv[1], argv + 1);
		break;
	default:
		break;
	}

	/*
	    Recomendado por el manual : "When the tracee is in ptrace-stop, the
	   tracer can read and write data to the tracee using informational
	   commands." Una vez hecho el wait, sera necesario indicarle a ptrace
	   con los siguente flags:
	    - PTRACE_SETOPTIONS que permitira que tipo de data queremos obtener
	    - PTRACE_O_TRACESYSGOOD es la mascara que utilizaremos para que el
	   tracer pueda distinguir entre los traps producidor por las syscalls
	*/
	waitpid(pid, 0, 0);
	ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_TRACESYSGOOD);

	/*
	    Con un loop iremos recorriendo los llamados de las syscals
	    1) PTRACE_SYSCALL: Nos permite que el trace se detenga cuando una
	   syscall es llamada a ejecutarse. En caso de error, se hara un exit 2)
	   PTRACE_GETREGS y PTRACE_GET_SYSCALL_INFO: Podremos obtener la
	   informacion de la syscall y de los registros 3) Sera necesario hace
	   otro llamado a PTRACE_SYSCALL para que continue con la proxima
	   intruccion

	*/
	for (;;) {
		/*
		    Con el primer PTRACE_SYSCALL se hacen los arreglos para
		   que el proceso hijo se detenga al entrar a una syscall, y con
		   el segundo para que se detenga al salir de esa syscall. En la
		   primer oportunidad pueden obtenerse los argumentos de la
		   llamada al sistema, y en la segunda, el valor de retorno.
		*/
		if (ptrace(PTRACE_SYSCALL, pid, 0, 0) == -1) {
			log_(L_ERROR, "+++ exited with 0 +++");
		}
		if (waitpid(pid, 0, 0) == -1) {
			log_(L_ERROR, "Error...");
		}

		struct user_regs_struct regs;
		ptrace(PTRACE_GETREGS, pid, 0, &regs);

		if (regs.orig_rax == 252)
			exit(0);  // exit_group syscall

		if (ptrace(PTRACE_SYSCALL, pid, 0, 0) == -1)
			log_(L_ERROR, "+++ exited with 0 +++");
		waitpid(pid, 0, 0);
		struct ptrace_syscall_info sys_info;
		ptrace(PTRACE_GET_SYSCALL_INFO,
		       pid,
		       sizeof(struct ptrace_syscall_info),
		       &sys_info);
		if (regs.orig_rax < 335) {
			print_info(sys_info, regs);
		}
	}
}


void
print_info(struct ptrace_syscall_info sys_info, struct user_regs_struct regs)
{
	printf("Nombre syscall: " BLU "%s " RESET ",", _syscall[regs.orig_rax]);
	printf("Return Value: " BLU "%lld\n" RESET, sys_info.exit.rval);
}

void
log_(unsigned int type, const char *message)
{
	if (type == L_ERROR) {
		fprintf(stderr, "%s\n", message);
		exit(L_ERROR);
	}

	if (type == L_WARNING)
		fprintf(stderr, "%s\n", message);

	else if (type == L_LOG)
		fprintf(stdout, "%s\n", message);

	return;
}
