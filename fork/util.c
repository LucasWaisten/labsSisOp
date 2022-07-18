#include "util.h"

int
_fork(void)
{
	int res = fork();

	if (res < 0) {
		perror("Error en FORK");
		_exit(-1);
	}
	return res;
}

ssize_t
_read(int fd, void *buf, size_t count)
{
	ssize_t res = read(fd, buf, count);

	if (res < 0) {
		perror("Error en READ");
		_exit(-1);
	}
	return res;
}
