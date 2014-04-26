/* Copyright (C) 2014-2014. All Rights Reserved. */

#include <stdio.h>
#include <string.h>

/* [Scenario 1]
 *	This is a simple scenario which user try to input key and
 *	program check the key correct or not.
 *
 * [Solution]
 *	You can use tool *strings* to get all string occurred on this
 *	program. WLOG, the password will occurred in your eyes.
 *
 * [How to build]
 *	gcc -Wall -o scenario1 scenario1.c
 */
#define PWD "PASSWORD"
int main(int argc, char *argv[])
{
	int i=0;
	char input[BUFSIZ] = {0};

	printf("** There is no HINT! **\n");
	printf("Key: ");
	scanf("%32s", input);
	if (strlen(PWD) != strlen(input)) {
		goto ERR;
	} else {
		for (i=0; i<strlen(input); ++i) {
			if (input[i] != PWD[i]) goto ERR; 
		}
		goto DONE;
	}
ERR:
	printf("Login faild\n");
	return 1;
DONE:
	printf("Pwn!\n");
	return 0;
}

