#ifndef TP_FORK_UTIL_H
#define TP_FORK_UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

typedef char string[64];

int _fork(void);

ssize_t _read(int fd, void *buf, size_t count);


#endif