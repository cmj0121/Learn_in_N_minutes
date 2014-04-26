/* Copyright (C) 2014-2014. All Rights Reserved. */

#include <stdio.h>
#include <string.h>

/* [Scenario 3]
 *	This program will show how to handle arguments when program run.
 *
 *
 * [How to build]
 *	gcc -Wall -o scenario3 scenario3.c
 */
int main(int argc, char *argv[])
{
	int i = 0;
	char pwd[] = {0x50, 0, 0x41, 0, 0x53, 0, 0X53, 0, 0x57, 0, 0X4F, 0, 0x52, 0, 0x44};


	printf("** HINT: You need put arguments when you execute. **\n");
	if (8 == strlen(argv[0])) {
		for (i=0; i< strlen(argv[0]); ++i) {
			if (argv[0][i] != pwd[i*2]) {
				printf("login faild\n");
				return 1;
			}
		}
		printf("pwn!\n");
		return 0;
	} else {
		printf("login faild\n");
		return 1;
	}
}

