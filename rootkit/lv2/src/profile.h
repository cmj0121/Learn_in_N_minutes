/* Copyright (C) 2014 cmj. All right reserved. */
#ifndef __PROFILE_H__
#define __PROFILE_H__


int profile_readdir;
int profile_readdir64;
int profile_open;
int profile_xstat;
int profile_lxstat;
int profile_fxstat;

/* The profile and debug functions */
#ifdef DEBUG
#define PROFILE(fn) \
	do {\
		profile_##fn += 1; \
		fprintf(stderr, "%-10s %d\n", #fn, profile_##fn); \
	} while (0)
#else
#define PROFILE(fn)
#endif

#ifndef BLOCK_LIST
#define BLOCK_LIST ""
#endif /* BLOCK_LIST */

#endif /* __PROFILE_H__ */

