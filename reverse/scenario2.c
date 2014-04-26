/* Copyright (C) 2014-2014. All Rights Reserved. */

#include <stdio.h>
#include <string.h>

/* [Scenario 2]
 *	This program is enhance version for scenario1, it avoid string
 *	occurred in binary, i.e. using string will not workable.
 *
 * [Solution]
 *	Since we cannot get the key from string, we need to analysis brinary
 *	directly. we can dump assembly code by objdump / otool and see the
 *	raw code.
 *
 *
 * [How to build]
 *	gcc -Wall -o scenario2 scenario2.c
 */
int main(int argc, char *argv[])
{
	int i = 0;
	char input[BUFSIZ] = {0};
	char pwd[] = {0x50, 0, 0x41, 0, 0x53, 0, 0X53, 0, 0x57, 0, 0X4F, 0, 0x52, 0, 0x44};

	printf("** HINT: Assembly code **\n");
	printf("Key: ");
	scanf("%32s", input);
	if (8 == strlen(input)) {
		for (i=0; i< strlen(input); ++i) {
			if (input[i] != pwd[i*2]) {
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

