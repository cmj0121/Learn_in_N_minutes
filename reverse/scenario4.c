/* Copyright (C) 2014-2014. All Rights Reserved. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* [Scenario 4]
 *	This program will introduce how to load extra file.
 *
 *
 * [How to build]
 *	gcc -Wall -o scenario3 scenario3.c
 */
int main(int argc, char *argv[])
{
	int i = 0;
	char pwd[] = {0x50, 0, 0x41, 0, 0x53, 0, 0X53, 0, 0x57, 0, 0X4F, 0, 0x52, 0, 0x44};
	char *env = NULL, input[BUFSIZ] = {0};
	FILE *fd;

	printf("** HINT: Which file should you include. **\n");
	env = getenv("REVERSE_KEY_FILE");

	if (NULL == env) {
		goto ERR;
	} else if (0 == access(env, R_OK)) {
		if (NULL == (fd = fopen(env, "r"))) {
			goto ERR;
		}
		fread(input, 1, BUFSIZ, fd);
		fclose(fd);

		/* Remove extra \n in file */
		if (0 < strlen(input) && input[strlen(input)-1]) {
			input[strlen(input)-1] = 0x0;
		}

		if (strlen(input) == 8) {
			for (i=0; i<strlen(input); ++i) {
				if (input[i] != pwd[i*2]) goto ERR;
			}
			goto DONE;
		}
	}
ERR:
	printf("Login faild\n");
	return 1;
DONE:
	printf("Pwn!\n");
	return 0;
}

