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
	int path1_end = path->len - 1;
	int path2_start = 0;

	if (path->str[path1_end] == PATH_SEP) {
		path1_end--;
	}
	if (path_string[0] == PATH_SEP) {
		path2_start++;
	}
	size_t newlen = path1_end + 2 + strlen(path_string) - path2_start;
	char *newbuf = (char *)realloc(path->str, newlen);
	/* strncpy(newbuf, path->str, path1_end + 1);
	newbuf[path1_end + 1] = '/';
	strcpy(&(newbuf[path1_end + 2]), &(path_string[path2_start])); */
	snprintf(newbuf, newlen, "%s%c%s", /* TODOOOOO it's a noisy environment right now...*/);
}

path_t *path_normalize(path_t *path)
{

}

void path_dispose(path_t *path)
{

}
