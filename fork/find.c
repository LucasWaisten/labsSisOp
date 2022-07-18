#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>

#ifndef PATH_MAX
#define PATH_MAX 100
#endif

#define SENSITIVE 1
#define INSENSITIVE -1

#define _GNU_SOURCE
char *strcasestr(const char *haystack, const char *needle);

void findPathInsensitive(DIR *pDir, char *cadena, char *d_path);
void findPathSenitive(DIR *pDir, char *cadena, char *d_path);

char path_directorio[PATH_MAX] = " ";
void
findPathInsensitive(DIR *pDir, char *cadena, char *d_path)
{
	struct dirent *pDirent;
	if ((pDirent = readdir(pDir)) == NULL) {
		return;
	}

	if (strcmp(pDirent->d_name, "..") != 0 &&
	    strcmp(pDirent->d_name, ".") != 0) {
		if (pDirent->d_type == DT_DIR) {
			char subd_path[PATH_MAX];  // Path del subdirectiorio posicionado

			strcpy(subd_path,
			       d_path);  // Hago una copia para no perder su referencia
			strcat(d_path, pDirent->d_name);

			if (strcasestr(pDirent->d_name, cadena)) {
				printf("%s\n", d_path);
			}
			strcat(d_path, "/");

			DIR *pDir_sub = fdopendir(
			        openat(dirfd(pDir), pDirent->d_name, O_DIRECTORY));
			findPathInsensitive(pDir_sub, cadena, d_path);

			strcpy(d_path,
			       subd_path);  // Vuelvo a copiar el subdirectorio en el path principal
			closedir(pDir_sub);

		} else if (strcasestr(pDirent->d_name, cadena)) {
			printf("%s%s\n", d_path, pDirent->d_name);
		}
	}
	findPathInsensitive(pDir, cadena, d_path);
}
void
findPathSenitive(DIR *pDir, char *cadena, char *d_path)
{
	struct dirent *pDirent;
	if ((pDirent = readdir(pDir)) == NULL) {
		return;
	}

	if (strcmp(pDirent->d_name, "..") != 0 &&
	    strcmp(pDirent->d_name, ".") != 0) {
		if (pDirent->d_type == DT_DIR) {
			char subd_path[PATH_MAX];  // Path del subdirectiorio posicionado

			strcpy(subd_path,
			       d_path);  // Hago una copia para no perder su referencia
			strcat(d_path, pDirent->d_name);

			if (strstr(pDirent->d_name, cadena)) {
				printf("%s\n", d_path);
			}
			strcat(d_path, "/");

			DIR *pDir_sub = fdopendir(
			        openat(dirfd(pDir), pDirent->d_name, O_DIRECTORY));
			findPathSenitive(pDir_sub, cadena, d_path);

			strcpy(d_path,
			       subd_path);  // Vuelvo a copiar el subdirectorio en el path principal
			closedir(pDir_sub);

		} else if (strstr(pDirent->d_name, cadena)) {
			printf("%s%s\n", d_path, pDirent->d_name);
		}
	}
	findPathSenitive(pDir, cadena, d_path);
}

int
main(int argc, char *argv[])
{
	if (argc < 2 || argc > 3) {
		printf("Cantidad de parametros incorrectos\n");
		return 0;
	}
	DIR *pDir;

	char d_path[PATH_MAX] = "";

	pDir = opendir(".");
	if (pDir == NULL) {
		printf("Cannot open directory\n");
		return 1;
	}

	if (strcmp(argv[1], "-i") == 0) {
		findPathInsensitive(pDir, argv[2], d_path);
	} else {
		findPathSenitive(pDir, argv[1], d_path);
	}
	closedir(pDir);
	return 0;
}