/* Copyright (C) 2014 cmj. All right reserved. */
#ifndef __DEBUG_H_
#define __DEBUG_H_

#define MAGIC_NAME	"/tmp/PyMonitor"

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
	do { \
		fprintf(stderr, "\033[36m[%s #%04d (%d)] \033[33m%-8s\033[m "fmt "\n", \
			__FILE__, __LINE__, getpid(), __func__, ##__VA_ARGS__); \
	} while (0)
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
		snprintf(msg, sizeof(msg), "%d:%s: " fmt, \
			getpid(), __func__, ##__VA_ARGS__); \
		write(__log_fd_, msg, strlen(msg)); \
	} while(0)
char __cmdline__[BUFSIZ];
void *__libc__;
char **_orig_environ;

#endif /* __DEBUG_H_ */

