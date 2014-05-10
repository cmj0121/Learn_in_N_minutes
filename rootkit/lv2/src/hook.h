/* Copyright (C) 2014 cmj. All right reserved. */
#ifndef __HOOK_H_
#define __HOOK_H_


/* Location for libc library */
#ifndef LIBC_SHARDLIB
#define LIBC_SHARDLIB "/usr/lib/libc.so.6"
#endif /* LIBC_SHARDLIB */

/* Define where the socket we bind and sendto */
#ifndef LISTEN_SOCKET
#define LISTEN_SOCKET "/tmp/listen.sock"
#endif /* LISTEN_SOCKET */

#ifdef  DEBUG
#define DEBUG_MSG(fmt, ...) \
	fprintf(stderr, "\033[36m[%s #%04d (%d)] \033[33m%-8s\033[m "fmt "\n", \
		__FILE__, __LINE__, getpid(), __func__, ##__VA_ARGS__)
#else
#define DEBUG_MSG(fmt, ...)
#endif /* DEBUG */


#include <string.h>
int __log_fd_;
#define LOG_MSG(fmt, ...) \
	do { \
		char msg[BUFSIZ] = {0}; \
		DEBUG_MSG(fmt, ##__VA_ARGS__); \
		if (0 > __log_fd_) break; \
		snprintf(msg, sizeof(msg), "%s: " fmt, __func__, ##__VA_ARGS__); \
		write(__log_fd_, msg, strlen(msg)); \
	} while(0)
char __cmdline__[BUFSIZ];
void *__libc__;
char **_orig_environ;

/* Load the recursive syscall on following initail function */
__attribute__((constructor)) void begin(void);
__attribute__((destructor)) void after(void);

/* ==== unistd.h ==== */
static void _init_unistd_(void);
static int (*_execl)   (const char *__path, const char *__arg, ...);
static int (*_execle)  (const char *__path, const char *__arg, ...);
static int (*_execlp)  (const char *__file, const char *__arg, ...);
static int (*_execv)   (const char *__path, char *const __argv[]);
static int (*_execve)  (const char *__path, char *const __argv[], char *const __envp[]);
static int (*_execvp)  (const char *__file, char *const __argv[]);

/* ==== fcntl.h ==== */
static void _init_fcntl_(void);
static int (*_open)    (const char *__file, int __oflag, ...);
#ifdef __USE_LARGEFILE64
static int (*_open64)  (const char *__file, int __oflag, ...);
#endif /* __USE_LARGEFILE64 */

/* ==== sys/stat.h ==== */
static void _init_sys_stat_(void);
static int (*_chmod)   (const char *__file, __mode_t __mode);
static int (*_lchmod)  (const char *__file, __mode_t __mode);
static int (*_fchmod)  (int __fd, __mode_t __mode);
static int (*_mkdir)   (const char *__path, __mode_t __mode);
#ifndef __USE_FILE_OFFSET64
static int (*_xstat)   (int __ver, const char *__filename, struct stat *__stat_buf);
static int (*_fxstat)  (int __ver, int __fildes, struct stat *__stat_buf);
static int (*_lxstat)  (int __ver, const char *__filename, struct stat *__stat_buf);
#  ifdef __USE_LARGEFILE64
static int (*_xstat64) (int __ver, const char *__filename, struct stat64 *__stat_buf);
static int (*_fxstat64)(int __ver, int __fildes, struct stat64 *__stat_buf);
static int (*_lxstat64)(int __ver, const char *__filename, struct stat64 *__stat_buf);
#  endif /* __USE_LARGEFILE64 */
#endif /* __USE_FILE_OFFSET64 */

/* ==== stdlib.h ==== */
static void _init_stdlib_(void);
static int   (*_system)  (const char *__command);
static char* (*_getenv)  (const char *__name);

/* ==== sys/socket.h ==== */
static void _init_sys_socket_(void);
static int (*_bind)     (int __fd, __CONST_SOCKADDR_ARG __addr, socklen_t __len);
static int (*_connect)  (int __fd, __CONST_SOCKADDR_ARG __addr, socklen_t __len);
static int (*_accept)   (int __fd, __SOCKADDR_ARG __addr, socklen_t *__restrict __addr_len);

#endif /* __HOOK_H_ */

