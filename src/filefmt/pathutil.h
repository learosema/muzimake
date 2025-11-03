#ifndef __PATHUTIL_H__
#define __PATHUTIL_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "helper/list.h"

#if defined __DOS__ || defined __WINDOWS__
#define PATH_SEP "\\"
#else
#define PATH_SEP "/"
#endif

typedef struct path_s {
	linked_list_t *list;
	char *string;
} path_t;

path_t *path_create(char *path_string);
path_t *path_join(path_t *path, char *path_string);
path_t *path_normalize(path_t *path);
void path_dispose(path_t *path);

#ifdef __cplusplus
}
#endif

#endif
