#ifndef DIRECTORY_H
#define DIRECTORY_H

#ifdef __cplusplus
extern "C" {
#endif

int directory_is_file(const char *directory, const char *name);
int directory_is_dir(const char *directory, const char *name);

#ifdef __cplusplus
}
#endif

#endif
