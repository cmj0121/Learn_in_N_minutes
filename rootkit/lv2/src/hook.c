/* Copyright (C) 2014 cmj. All right reserved. */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "hook.h"

/* Constructor and descructor
 *
 * Do anything override syscall on this function.
 */

static char **hidden_env() {
	int i, numEnv = 0;
	char **ret = NULL, **ptr;

	for (ret = environ; (*ret); ++ret) numEnv ++;
	if (NULL== (ret = calloc(sizeof(char *), numEnv))) {
		DEBUG_MSG("Cannot calloc for environment");
		goto ERR;
	} else {
		i = 0;
		for (ptr = environ; (*ptr); ++ptr) {
			if (0 == strncmp((*ptr), "LD_PRELOAD", 10)) continue;
			ret[i] = calloc(sizeof(char), strlen(*ptr));
			snprintf(ret[i], strlen(*ptr), "%s", *ptr);
			i++;
		}
	}
ERR:
	return ret;
}
static void _init_(void) {
	/* Load the override syscall funtion */
	_init_unistd_();
	_init_fcntl_();
	_init_sys_stat_();
	_init_stdlib_();
	_init_sys_socket_();
}

__attribute__((constructor)) void begin(void) {
	FILE *file = NULL;
	struct sockaddr_un addr;

	__log_fd_ = -1;

	/* Replace all libc method */
	_init_();

	/* Bind to client if possbile */
	if (NULL == (__libc__ = dlopen(LIBC_SHARDLIB, RTLD_LAZY))) {
		DEBUG_MSG("Cannot load libc %s", LIBC_SHARDLIB);
	} else if(0 > (__log_fd_ = socket(PF_UNIX, SOCK_STREAM, 0))) {
		DEBUG_MSG("Cannot create domain socket %m");
	} else {
		addr.sun_family = AF_UNIX;
		snprintf(addr.sun_path, sizeof(addr.sun_path), LISTEN_SOCKET);
		if (0 != connect(__log_fd_, (struct sockaddr *)&addr, sizeof(struct sockaddr_un))) {
			DEBUG_MSG("Cannot connect to %s (%m)", LISTEN_SOCKET);
		} else {
			DEBUG_MSG("Success bind to %s", LISTEN_SOCKET);
		}
	}

	/* Get he information for currnent command */
	if (NULL == (file = fopen("/proc/self/cmdline", "r"))) {
		DEBUG_MSG("Cannot open cmdline on proc (%m)");
	} else {
		fread(__cmdline__, sizeof(__cmdline__), 1, file);
		fclose(file);
		file = NULL;
	}


	/* Hidden environment for env */
	if (0 == strcmp("/usr/bin/env", __cmdline__) || 0 == strcmp("env", __cmdline__)) {
		_orig_environ = environ;
		environ = hidden_env();
		DEBUG_MSG("Hidden environment for env");
	}

	LOG_MSG("%s", __cmdline__);
	if (NULL != file) {
		fclose(file);
		file = NULL;
	}
	return ;
}
__attribute__((destructor)) void after(void) {
	if (0 <= __log_fd_) {
		close(__log_fd_);
		__log_fd_ = -1;
	}

	if (_orig_environ) {
		environ = _orig_environ;
		_orig_environ = NULL;
	}
	if (NULL != __libc__) {
		dlclose(__libc__);
		__libc__ = NULL;
	}
	LOG_MSG("%s", __cmdline__);

	DEBUG_MSG("Exit and close domain socket on <%s>", __cmdline__);
}

/* ==== unistd ==== */
static void _init_unistd_(void) {
	_execl  = dlsym(RTLD_NEXT, "execl");
	_execle = dlsym(RTLD_NEXT, "execle");
	_execlp = dlsym(RTLD_NEXT, "execlp");
	_execv  = dlsym(RTLD_NEXT, "execv");
	_execve = dlsym(RTLD_NEXT, "execve");
	_execvp = dlsym(RTLD_NEXT, "execvp");
}
int execl (const char *__path, const char *__arg, ...) {
	int _ = -1;
	va_list args;

	va_start(args, __arg);
	if (_execl) {
		_ = _execl(__path, __arg, args);
		LOG_MSG("%s", __path);
	}
	va_end(args);
	return _;
}
int execle (const char *__path, const char *__arg, ...) {
	int _ = -1;
	va_list args;

	va_start(args, __arg);
	if (_execle) {
		_ = _execle(__path, __arg, args);
		LOG_MSG("%s", __path);
	}
	va_end(args);
	return _;
}
int execlp (const char *__file, const char *__arg, ...) {
	int _ = -1;
	va_list args;

	va_start(args, __arg);
	if (_execlp) {
		_ = _execlp(__file, __arg, args);
		LOG_MSG("%s", __file);
	}
	va_end(args);
	return _;
}
int execv (const char *__path, char *const __argv[]) {
	int _ = -1;

	if (_execv) {
		_ = _execv(__path, __argv);
		LOG_MSG("%s", __path);
	}
	return _;
}
int execve (const char *__path, char *const __argv[], char *const __envp[]) {
	int _ = -1;

	if (_execve) {
		_ = _execve(__path, __argv, __envp);
		LOG_MSG("%s", __path);
	}
	return _;
}
int execvp (const char *__file, char *const __argv[]) {
	int _ = -1;

	if (_execvp) {
		_ = _execvp(__file, __argv);
		LOG_MSG("%s", __file);
	}
	return _;
}

/* ==== fcntl.h ==== */
static void _init_fcntl_(void) {
	_open   = dlsym(RTLD_NEXT, "open");
	_open64 = dlsym(RTLD_NEXT, "open64");
	_dlopen = dlsym(RTLD_NEXT, "dlopen");
}
int open (const char *__file, int __oflag, ...) {
	int _ = -1;
	va_list args;

	va_start(args, __oflag);
	if (_open) {
		_ = _open(__file, __oflag, args);
		LOG_MSG("%s", __file);
	}
	va_end(args);
	return _;
}
#ifdef __USE_LARGEFILE64
int open64 (const char *__file, int __oflag, ...) {
	int _ = -1;
	va_list args;

	va_start(args, __oflag);
	if (_open64) {
		_ = _open64(__file, __oflag, args);
		LOG_MSG("%s", __file);
	}
	va_end(args);
	return _;
}
#endif /* __USE_LARGEFILE64 */
void *dlopen (const char *__file, int __mode) {
	void *_ = NULL;

	if (_dlopen) {
		_ = _dlopen(__file, __mode);
		LOG_MSG("%s", __file);
	}

	return _;
}

/* ==== sys/stat.h ==== */
static void _init_sys_stat_(void) {
	_chmod    = dlsym(RTLD_NEXT, "chmod");
	_lchmod   = dlsym(RTLD_NEXT, "lchmod");
	_fchmod   = dlsym(RTLD_NEXT, "fchmod");
	_mkdir    = dlsym(RTLD_NEXT, "mkdir");
#ifndef __USE_FILE_OFFSET64
	_xstat    = dlsym(RTLD_NEXT, "__xstat");
	_fxstat   = dlsym(RTLD_NEXT, "__fxstat");
	_lxstat   = dlsym(RTLD_NEXT, "__lxstat");
#  ifdef __USE_LARGEFILE64
	_xstat64  = dlsym(RTLD_NEXT, "__xstat64");
	_fxstat64 = dlsym(RTLD_NEXT, "__fxstat64");
	_lxstat64 = dlsym(RTLD_NEXT, "__lxstat64");
#  endif /* __USE_LARGEFILE64 */
#endif /* __USE_FILE_OFFSET64 */
}
int chmod (const char *__file, __mode_t __mode) {
	int _ = -1;

	if (_chmod) {
		_ = _chmod(__file, __mode);
		LOG_MSG("%s", __file);
	}
	return _;
}
int lchmod (const char *__file, __mode_t __mode) {
	int _ = -1;

	if (_lchmod) {
		_ = _lchmod(__file, __mode);
		LOG_MSG("%s", __file);
	}
	return _;
}
int fchmod (int __fd, __mode_t __mode) {
	int _ = -1;

	if (_fchmod) {
		_ = _fchmod(__fd, __mode);
		LOG_MSG("%d", __fd);
	}
	return _;
}
int mkdir (const char *__path, __mode_t __mode) {
	int _ = -1;

	if (_mkdir) {
		_ = _mkdir(__path, __mode);
		LOG_MSG("%s", __path);
	}
	return _;
}
#ifndef __USE_FILE_OFFSET64
int __xstat (int __ver, const char *__filename, struct stat *__stat_buf) {
	int _ = -1;

	if (_xstat) {
		_ = _xstat(__ver, __filename, __stat_buf);
		LOG_MSG("%s", __filename);
	}
	return _;
}
int __fxstat (int __ver, int __fildes, struct stat *__stat_buf) {
	int _ = -1;

	if (_fxstat) {
		_ = _fxstat(__ver, __fildes, __stat_buf);
		LOG_MSG("%d", __fildes);
	}
	return _;
}
int __lxstat (int __ver, const char *__filename, struct stat *__stat_buf) {
	int _ = -1;

	if (_lxstat) {
		_ = _lxstat(__ver, __filename, __stat_buf);
		LOG_MSG("%s", __filename);
	}
	return _;
}
#  ifdef __USE_LARGEFILE64
int __xstat64 (int __ver, const char *__filename, struct stat64 *__stat_buf) {
	int _ = -1;

	if (_xstat64) {
		_ = _xstat64(__ver, __filename, __stat_buf);
		LOG_MSG("%s", __filename);
	}
	return _;
}
int __fxstat64 (int __ver, int __fildes, struct stat64 *__stat_buf) {
	int _ = -1;

	if (_fxstat64) {
		_ = _fxstat64(__ver, __fildes, __stat_buf);
		LOG_MSG("%d", __fildes);
	}
	return _;
}
int __lxstat64 (int __ver, const char *__filename, struct stat64 *__stat_buf) {
	int _ = -1;

	if (_lxstat64) {
		_ = _lxstat64(__ver, __filename, __stat_buf);
		LOG_MSG("%s", __filename);
	}
	return _;
}
#  endif /* __USE_LARGEFILE64 */
#endif /* __USE_FILE_OFFSET64 */

/* ==== stdlib.h ==== */
static void _init_stdlib_(void) {
	_system   = dlsym(RTLD_NEXT, "system");
	_getenv   = dlsym(RTLD_NEXT, "getenv");
}
int system (const char *__command) {
	int _ = -1;

	if (_system) {
		_ = _system(__command);
		LOG_MSG("%s", __command);
	}
	return _;
}
char* getenv (const char *__name) {
	char *_ = NULL;

	if (_getenv) {
		_ = _getenv(__name);
		LOG_MSG("%s", __name);
	}
	return _;
}

/* ==== sys/socket.h ==== */
static void _init_sys_socket_(void) {
	_bind     = dlsym(RTLD_NEXT, "bind");
	_connect  = dlsym(RTLD_NEXT, "connect");
	_accept   = dlsym(RTLD_NEXT, "accept");
}
int bind (int __fd, __CONST_SOCKADDR_ARG __addr, socklen_t __len) {
	int _ = -1;
	const struct sockaddr_in **addr = NULL;

	if (_bind) {
		_ = _bind(__fd, __addr, __len);
		addr = (const struct sockaddr_in **)&__addr;
		LOG_MSG("%s:%u", inet_ntoa((*addr)->sin_addr), (*addr)->sin_port);
	}
	return _;
}
int connect (int __fd, __CONST_SOCKADDR_ARG __addr, socklen_t __len) {
	int _ = -1;
	const struct sockaddr_in **addr = NULL;

	if (_connect) {
		_ = _connect(__fd, __addr, __len);
		addr = (const struct sockaddr_in **)&__addr;
		if (AF_UNIX == (*addr)->sin_family) {
			LOG_MSG("%hu:%s %m",
				AF_UNIX,
				(*((const struct sockaddr_un **)&__addr))->sun_path);
		} else if (AF_INET == (*addr)->sin_family) {
			LOG_MSG("%hu:%s:%hu %m",
				AF_UNIX,
				inet_ntoa((*addr)->sin_addr),
				(*addr)->sin_port);
		}
	}
	return _;
}
int accept (int __fd, __SOCKADDR_ARG __addr, socklen_t *__restrict __addr_len) {
	int _ = -1;
	struct sockaddr_in **addr = NULL;

	if (_accept) {
		_ = _accept(__fd, __addr, __addr_len);
		addr = (struct sockaddr_in **)&__addr;
		LOG_MSG("%s:%u", inet_ntoa((*addr)->sin_addr), (*addr)->sin_port);
	}
	return _;
}

