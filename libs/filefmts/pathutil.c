#include "pathutil.h"
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#if defined __DOS__ && defined __WATCOM__
#include <dos.h>
#else
#include <unistd.h>
#endif

/* Sicherheitsgrenze für path_getcwd: kein reales Dateisystem braucht einen
 * cwd-Puffer in dieser Größenordnung (Linux ~4096, Windows ~32767 Zeichen).
 * Bewusst unter 65536 gewählt, damit der Wert selbst auf Plattformen mit
 * 16-Bit size_t (max. 65535, z. B. DOS/Watcom Real Mode) noch gültig ist -
 * verhindert eine Endlosschleife, falls "bufsize *= 2" sonst überliefe. */
#define PATH_GETCWD_MAX_BUFSIZE ((size_t)32768)

path_t *path_create(const char *path_string)
{
	if (path_string == NULL) {
		return NULL;
	}
	path_t *result = (path_t *)malloc(sizeof(path_t));
	if (result == NULL) {
		return NULL;
	}
	result->len = strlen(path_string);
	result->str = (char*)malloc(result->len + 1);
	if (result->str == NULL) {
		free(result);
		return NULL;
	}

	memcpy(result->str, path_string, result->len + 1);
	return result;
}

path_t *path_join(path_t *path, const char *path_string)
{
	if (path == NULL || path_string == NULL) {
		return path;
	}

	/* path1_end = Anzahl der Zeichen von path1, die übernommen werden
	 * (ohne evtl. abschließenden Separator). size_t statt int, damit
	 * ein leerer path (len == 0) kein Underflow auf str[-1] auslöst. */
	size_t path1_end = path->len;
	if (path1_end > 0) {
		char path1_tail = path->str[path1_end - 1];
		if (path1_tail == PATH_SEP || path1_tail == OTHER_PATH_SEP) {
			path1_end--;
		}
	}

	size_t path2_start = 0;
	char path2_head = path_string[0];
	if (path2_head == PATH_SEP || path2_head == OTHER_PATH_SEP) {
		path2_start++;
	}

	size_t suffix_len = strlen(path_string) - path2_start;
	bool need_sep = (path1_end > 0); /* kein führender Separator bei leerem path1 */
	size_t newlen = path1_end + (need_sep ? 1 : 0) + suffix_len + 1; /* +1 für '\0' */

	char *newbuf = (char *)realloc(path->str, newlen);
	if (newbuf == NULL) {
		return NULL; /* realloc fehlgeschlagen, path bleibt unverändert nutzbar */
	}

	size_t write_pos = path1_end;
	if (need_sep) {
		newbuf[write_pos] = PATH_SEP;
		write_pos++;
	}
	memmove(&newbuf[write_pos], &path_string[path2_start], suffix_len + 1); /* inkl. '\0' */

	for (size_t i = 0; i < newlen - 1; i++) {
		if (newbuf[i] == OTHER_PATH_SEP) {
			newbuf[i] = PATH_SEP;
		}
	}

	path->str = newbuf;
	path->len = newlen - 1;   /* newlen enthält den Platz für '\0', len selbst nicht */
	return path;
}

path_t *path_getcwd(void)
{
	size_t bufsize = 256;
	char *buf = (char *)malloc(bufsize);
	if (buf == NULL) {
		return NULL;
	}

	for (;;) {
#if defined __DOS__ && defined __WATCOM__
		char *ok = _getcwd(buf, (int)bufsize);
#else
		char *ok = getcwd(buf, bufsize);
#endif
		if (ok != NULL) {
			break;
		}
		if (errno != ERANGE) {
			/* echter Fehler (z. B. Verzeichnis gelöscht, keine Berechtigung) */
			free(buf);
			return NULL;
		}
		/* Puffer war zu klein: verdoppeln und erneut versuchen.
		 * Erst prüfen, ob die Verdopplung selbst überlaufen würde oder
		 * die Sicherheitsgrenze überschreitet - sonst potentielle
		 * Endlosschleife (siehe Kommentar bei PATH_GETCWD_MAX_BUFSIZE). */
		if (bufsize > PATH_GETCWD_MAX_BUFSIZE / 2) {
			free(buf);
			return NULL;
		}
		bufsize *= 2;
		char *newbuf = (char *)realloc(buf, bufsize);
		if (newbuf == NULL) {
			free(buf);
			return NULL;
		}
		buf = newbuf;
	}

	path_t *result = (path_t *)malloc(sizeof(path_t));
	if (result == NULL) {
		free(buf);
		return NULL;
	}
	result->str = buf;
	result->len = strlen(buf);
	return result;
}

path_t *path_normalize(path_t *path)
{
	return NULL;
}

void path_dispose(path_t *path)
{
	if (path == NULL) {
		return;
	}
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

