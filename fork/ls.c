#define _GNU_SOURCE
#include "util.h"
#include <string.h>
#include <fcntl.h>
#include "util.h"
#include <pwd.h>
#include <time.h>
#include <limits.h>
#include <grp.h>

#define RESET "\x1B[0m"
#define BLU "\x1B[34m"
#define CYN "\x1B[36m"

void _ls(void);
char *_printperm(struct stat statbuf, char *perm);
void _printName(struct dirent *entry);
void _printTime(char *time);

int
main(int argc, char *argv[])
{
	// Corroboro que los parametros sean los correctos
	if (argc != 2) {
		perror("Comando incompleto,pruebe con './ls -al'");
		exit(EXIT_FAILURE);
	}
	if (strcmp(argv[1], "-al") != 0) {
		perror("Parametro incorrecto, pruebe con './ls -al'");
		exit(EXIT_FAILURE);
	} else {
		// Si todo da bien, llamo la funcion que simulara el comando
		_ls();
	}
	return 0;
}

void
_ls(void)
{
	// Abro el directorio actual
	DIR *dir = opendir(".");

	if (!dir) {
		perror("Error: directorio no encontrado\n");
		exit(-1);
	}
	// Voy leyendo cada entry
	struct dirent *entry;
	while ((entry = readdir(dir))) {
		// Stat me permite obtener toda la informacion de un directorio
		// lstat sirve para aquellos directorio que sean de tipo symbolicos o softlink
		struct stat statbuf;
		if (lstat(entry->d_name, &statbuf) < 0) {
			perror("Error en lstat");
			exit(EXIT_FAILURE);
		}
		char perm[11] = "----------";

		// Imprimo por pantalla la informacion que me piden
		printf("\e[4m                                                  "
		       "        "
		       "  \e[0m\n\n");

		printf("PERMS: %s \nNLINKS: %ld \nUID: %s \nPGID: %s "
		       "\nSIZE: %ld\n",
		       _printperm(statbuf, perm),
		       statbuf.st_nlink,
		       getpwuid(statbuf.st_uid)->pw_name,
		       getgrgid(statbuf.st_gid)->gr_name,
		       statbuf.st_size);
		_printTime(ctime(&statbuf.st_mtime));
		_printName(entry);
	}
	printf(" \n");
	closedir(dir);
}

void
_printTime(char *time)
{
	char *token;
	token = strtok(time, " ");
	token = strtok(NULL, " ");
	printf("LASTMD: %s ", token);

	token = strtok(NULL, " ");
	printf("%s ", token);

	token = strtok(NULL, " ");
	printf("%s ", token);

	token = strtok(NULL, " ");
	printf("%s", token);
}
void
_printName(struct dirent *entry)
{
	struct stat sb;
	lstat(entry->d_name, &sb);

	// Para imprimir el nombre tengo que distinguir si es hardlink o softlink

	if (S_ISDIR(sb.st_mode))
		printf("DIRENTNAME: " BLU "%s\n" RESET, entry->d_name);
	else if (S_ISLNK(sb.st_mode)) {
		char *linkname = malloc(sb.st_size + 1);
		if (linkname == NULL) {
			perror("Error en malloc");
			exit(-1);
		}
		// Si es un softlink sera necesario utilizar el readlink
		// Me permitira obtener la informacion sobre el link a que apunta
		ssize_t r = readlink(entry->d_name, linkname, sb.st_size + 1);
		if (r == -1) {
			perror("Error en readlink");
			exit(-1);
		}

		if (r > sb.st_size) {
			perror("Error en enlace simbolico");
			exit(-1);
		}

		linkname[r] = '\0';

		printf("DIRENTNAME: " CYN "%s" RESET " -> %s\n",
		       entry->d_name,
		       linkname);

		free(linkname);
	} else {
		printf("DIRENT NAME: "
		       "%s\n",
		       entry->d_name);
	}
};


char *
_printperm(struct stat statbuf, char *perm)
{
	// Para imprimir los permisos voy descantando haciendo un AND(&)
	// entre el mode de stat y la mascara de cada permiso
	if (S_ISDIR(statbuf.st_mode))
		perm[0] = 'd';
	else if (S_ISLNK(statbuf.st_mode)) {
		perm[0] = 'l';
	}

	if ((statbuf.st_mode & S_IRUSR) == S_IRUSR)
		perm[1] = 'r';
	if ((statbuf.st_mode & S_IWUSR) == S_IWUSR)
		perm[2] = 'w';
	if ((statbuf.st_mode & S_IXUSR) == S_IXUSR)
		perm[3] = 'x';
	if ((statbuf.st_mode & S_IRGRP) == S_IRGRP)
		perm[4] = 'r';
	if ((statbuf.st_mode & S_IWGRP) == S_IWGRP)
		perm[5] = 'w';
	if ((statbuf.st_mode & S_IXGRP) == S_IXGRP)
		perm[6] = 'x';
	if ((statbuf.st_mode & S_IROTH) == S_IROTH)
		perm[7] = 'r';
	if ((statbuf.st_mode & S_IWOTH) == S_IWOTH)
		perm[8] = 'w';
	if ((statbuf.st_mode & S_IXOTH) == S_IXOTH)
		perm[9] = 'x';

	return perm;
}