#include "exec.h"

int redir_cmd(struct execcmd *r, int *fd);
int fd_verification(char *file, int *fd, int entrada, int flags);
void pipe_cmd(int *fd, int fd_open, int fd_close, struct cmd *cmd);
// sets "key" with the key part of "arg"
// and null-terminates it
//
// Example:
//  - KEY=value
//  arg = ['K', 'E', 'Y', '=', 'v', 'a', 'l', 'u', 'e', '\0']
//  key = "KEY"
//
static void
get_environ_key(char *arg, char *key)
{
	int i;
	for (i = 0; arg[i] != '='; i++)
		key[i] = arg[i];

	key[i] = END_STRING;
}

// sets "value" with the value part of "arg"
// and null-terminates it
// "idx" should be the index in "arg" where "=" char
// resides
//
// Example:
//  - KEY=value
//  arg = ['K', 'E', 'Y', '=', 'v', 'a', 'l', 'u', 'e', '\0']
//  value = "value"
//
static void
get_environ_value(char *arg, char *value, int idx)
{
	size_t i, j;
	for (i = (idx + 1), j = 0; i < strlen(arg); i++, j++)
		value[j] = arg[i];

	value[j] = END_STRING;
}

// sets the environment variables received
// in the command line
//
// Hints:
// - use 'block_contains()' to
// 	get the index where the '=' is
// - 'get_environ_*()' can be useful here
static void
set_environ_vars(char **eargv, int eargc)
{
	for (int i = 0; i < eargc; i++) {
		int index = block_contains(eargv[i], '=');
		size_t n = strlen(eargv[i]);
		if (index > 1) {
			char key[index + 1];
			char value[n - index];
			get_environ_key(eargv[i], key);
			get_environ_value(eargv[i], value, index);
			if (setenv(key, value, 1) < 0)
				perror("Error en setenv");
		}
	}
}

// opens the file in which the stdin/stdout/stderr
// flow will be redirected, and returns
// the file descriptor
//
// Find out what permissions it needs.
// Does it have to be closed after the execve(2) call?
//
// Hints:
// - if O_CREAT is used, add S_IWUSR and S_IRUSR
// 	to make it a readable normal file
static int
open_redir_fd(char *file, int flags)
{
	int fd = open(file, flags, S_IWUSR | S_IRUSR);

	return fd;
}

static void
end_as_status(int status)
{
	if (WEXITSTATUS(status))
		_exit(WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		kill(getpid(), WTERMSIG(status));
	else
		_exit(status);
}

int
fd_verification(char *file, int *fd, int entrada, int flags)
{
	if (strlen(file) > 0) {
		if (block_contains(file, '&') == 0 &&
		    block_contains(file, '1') == 1) {
			if (dup2(fd[entrada - 1], entrada) < 0) {
				perror("Error al redirigir stderr");
				close(fd[entrada - 1]);
				return -1;
			}
		} else {
			fd[entrada] = open_redir_fd(file, flags);
			if (dup2(fd[entrada], entrada) < 0) {
				perror("Error al redirigir stderr");
				close(fd[entrada]);
				return -1;
			}
			if (entrada == 2)
				close(fd[entrada]);
		}
	}
	return 0;
}

int
redir_cmd(struct execcmd *r, int *fd)
{
	if (fd_verification(r->out_file, fd, STDOUT, O_CREAT | O_RDWR | O_TRUNC) < 0)
		return -1;
	if (fd_verification(r->in_file, fd, STDIN, O_RDONLY) < 0)
		return -1;
	if (fd_verification(r->err_file, fd, STDERR, O_CREAT | O_RDWR | O_TRUNC) < 0)
		return -1;

	if (execvp(*(r->argv), r->argv) == -1) {
		perror("Error");
		_exit(-1);
	}

	for (int i = 0; i < 3; i++) {
		close(fd[i]);
	}
	return 0;
}

void
pipe_cmd(int *fd, int fd_open, int fd_close, struct cmd *cmd)
{
	close(fd[fd_close]);
	dup2(fd[fd_open], fd_open);
	close(fd[fd_open]);
	exec_cmd(cmd);
}

// executes a command - does not return
//
// Hint:
// - check how the 'cmd' structs are definedextern int status;
// 	in types.h
// - casting could be a good option
void
exec_cmd(struct cmd *cmd)
{
	// To be used in the different cases
	struct execcmd *e;
	struct backcmd *b;
	struct execcmd *r;
	struct pipecmd *p;

	int fd[3];

	switch (cmd->type) {
	case EXEC:
		// spawns a command
		//
		// Your code here
		e = (struct execcmd *) cmd;
		if (e->eargc > 0)
			set_environ_vars(e->eargv, e->eargc);

		execvp(*(e->argv), e->argv);
		free_command(cmd);
		end_as_status(status);
		printf("Commands are not yet implemented\n");
		break;

	case BACK: {
		// runs a command in background
		//
		// Your code here
		b = (struct backcmd *) cmd;
		exec_cmd(b->c);

		printf("Background process are not yet implemented\n");
		break;
	}

	case REDIR: {
		// changes the input/output/stderr flow
		//
		// To check if a redirection has to be performed
		// verify if file name's length (in the execcmd struct)
		// is greater than zero
		//
		// Your code here

		r = (struct execcmd *) cmd;

		redir_cmd(r, fd);

		printf("Redirections are not yet implemented\n");
		free_command(cmd);
		end_as_status(status);
		break;
	}

	case PIPE: {
		// pipes two commands
		//
		// Your code here
		int fd[2];

		if (pipe(fd) == -1) {
			perror("pipe");
			exit(EXIT_FAILURE);
		}

		int left;
		int right;

		p = (struct pipecmd *) cmd;


		if ((left = fork()) == 0)
			pipe_cmd(fd, STDOUT, STDIN, p->leftcmd);
		else {
			if ((right = fork()) == 0)
				pipe_cmd(fd, STDIN, STDOUT, p->rightcmd);
			else {
				wait(NULL);
				close(fd[0]);
				close(fd[1]);
				wait(NULL);
			}
		}
		free_command(parsed_pipe);
		end_as_status(status);
		printf("Pipes are not yet implemented\n");

		// free the memory allocated
		// for the pipe tree structure
		// free_command(parsed_pipe);

		break;
	}
	}
}
