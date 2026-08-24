#include "pathutil.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#if defined __DOS__ && defined __WATCOM__
#include <dos.h>
#else
#include <unistd.h>
#endif

path_t *path_create(const char *path_string)
{
	path_t *result = (path_t *)malloc(sizeof(path_t));
	result->len = strlen(path_string);
	memcpy(result->str, path_string, result->len + 1);
	return result;
}

path_t *path_join(path_t *path, const char *path_string)
{
	if (path == NULL || path_string == NULL) {
		return path;
	}
	int path1_end = path->len - 1;
	int path2_start = 0;
	// NOT ROCK SOLID but works for now :D
	char path1_tail = path->str[path1_end];
	char path2_head = path_string[0];
	if (path1_tail == PATH_SEP || path1_tail == OTHER_PATH_SEP) {
		path1_end--;
	}
	if (path2_head == PATH_SEP || path2_head == OTHER_PATH_SEP) {
		path2_start++;
	}
	size_t newlen = path1_end + 2 + strlen(path_string) - path2_start;
	char *newbuf = (char *)realloc(path->str, newlen);
	strncpy(newbuf, path->str, path1_end + 1);
	newbuf[path1_end + 1] = PATH_SEP;
	strcpy(&(newbuf[path1_end + 2]), &(path_string[path2_start]));
	for (int i = 0; i < newlen; i++) {
		if (newbuf[i] == OTHER_PATH_SEP) {
			newbuf[i] = PATH_SEP;
		}
	}
	path->str = newbuf;
	path->len = newlen;
	return path;
}

path_t *path_normalize(path_t *path)
{
	return NULL;
}

void path_dispose(path_t *path)
{
	free(path->str);
	free(path);
}

bool path_chdir(const char *path_string)
{
	#if defined __DOS__ && defined __WATCOM__
	return ((_chdir(path_string)) == 0);
	#else
	return ((chdir(path_string)) == 0);
	#endif
	return false;
}
