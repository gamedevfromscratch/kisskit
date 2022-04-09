#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "kiss.h"

int main(void)
{
	char *space = NULL;
	FILE *fp = NULL;

	INFO("Hello");
	space = MALLOC(1000);
	space = REALLOC(space, 10000);
	fp = FOPEN("asdfasdf", "r");
	FCLOSE(fp);
	FREE(space);
	INFO("Bye");
	exit(0);
error:
	exit(1);
}
