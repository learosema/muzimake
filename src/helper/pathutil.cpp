#include "pathutil.h"
#include <stdlib.h>
#include <string.h>

path_t *path_create(char *path_string)
{
	path_t *result = (path_t *)malloc(sizeof(path_t));
	result->len = strlen(path_string);
	result->str = strdup(path_string);
	return result;
}

path_t *path_join(path_t *path, char *path_string)
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
