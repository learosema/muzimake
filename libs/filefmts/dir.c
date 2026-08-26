#include "dir.h"

#include <stdio.h>
#include <string.h>


#if defined(__DOS__) && defined(__WATCOMC__)

#include <direct.h>
#include <dos.h>

int directory_is_file(const char *directory, const char *name)
{
    struct find_t file;
    char path[512];

    snprintf(path, sizeof(path), "%s\\%s", directory, name);

    if (_dos_findfirst(path, _A_NORMAL | _A_RDONLY |
                       _A_HIDDEN | _A_SYSTEM | _A_ARCH,
                       &file) != 0)
        return 0;

    return (file.attrib & _A_SUBDIR) == 0 &&
           (file.attrib & _A_VOLID) == 0;
}

int directory_is_dir(const char *directory, const char *name)
{
    struct find_t file;
    char path[512];

    snprintf(path, sizeof(path), "%s\\%s", directory, name);

    if (_dos_findfirst(path, _A_SUBDIR, &file) != 0)
        return 0;

    return (file.attrib & _A_SUBDIR) != 0;
}


#elif defined(_WIN32)

#include <windows.h>

static int find_entry(const char *directory,
                      const char *name,
                      WIN32_FIND_DATAA *data)
{
    char path[4096];
    HANDLE handle;

    snprintf(path, sizeof(path), "%s\\%s", directory, name);

    handle = FindFirstFileA(path, data);

    if (handle == INVALID_HANDLE_VALUE)
        return 0;

    FindClose(handle);
    return 1;
}

int directory_is_file(const char *directory, const char *name)
{
    WIN32_FIND_DATAA data;

    if (!find_entry(directory, name, &data))
        return 0;

    return (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0;
}

int directory_is_dir(const char *directory, const char *name)
{
    WIN32_FIND_DATAA data;

    if (!find_entry(directory, name, &data))
        return 0;

    return (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
}


#else /* Linux / macOS / POSIX */

#include <sys/stat.h>

static int stat_entry(const char *directory,
                      const char *name,
                      struct stat *st)
{
    char path[4096];

    snprintf(path, sizeof(path), "%s/%s", directory, name);

    return stat(path, st) == 0;
}

int directory_is_file(const char *directory, const char *name)
{
    struct stat st;

    return stat_entry(directory, name, &st) &&
           S_ISREG(st.st_mode);
}

int directory_is_dir(const char *directory, const char *name)
{
    struct stat st;

    return stat_entry(directory, name, &st) &&
           S_ISDIR(st.st_mode);
}

#endif
