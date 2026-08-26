#ifndef __PATHUTIL_H__
#define __PATHUTIL_H__

#ifdef __cplusplus
extern "C" {
#endif
#include <stdlib.h>

#if defined __DOS__ || defined __WINDOWS__
#define PATH_SEP '\\'
#define OTHER_PATH_SEP '/'
#else
#define PATH_SEP '/'
#define OTHER_PATH_SEP '\\'
#endif

#if defined __DOS__ && defined __WATCOMC__
#include <direct.h>
#else
#include <dirent.h>
#endif

#include <stdbool.h>

typedef struct path_s {
	char *str;
	size_t len;
} path_t;

path_t *path_create(const char *path_string);
path_t *path_join(path_t *path, const char *path_string);
path_t *path_normalize(path_t *path);
path_t *path_getcwd(void);
void path_dispose(path_t *path);
bool path_chdir(const char *path_string);

#ifdef __cplusplus
}
#endif

#endif
