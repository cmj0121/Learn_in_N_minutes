/* Copyright (C) 2014 cmj. All right reserved. */
#ifndef __PROFILE_H__
#define __PROFILE_H__

/* The profile and debug functions */
#ifdef DEBUG
#define MSG(fmt, ...) fprintf(stderr, fmt"\n", ##__VA_ARGS__)
#else
#define MSG(fmt, ...)
#endif

#ifndef BLOCK_LIST
#define BLOCK_LIST ""
#endif /* BLOCK_LIST */

#endif /* __PROFILE_H__ */

