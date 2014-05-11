/* Copyright (C) 2014 cmj. All right reserved. */
#ifndef __HOOK_H_
#define __HOOK_H_

#include "debug.h"

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
static void* (*_dlopen) (const char *__file, int __mode);

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

