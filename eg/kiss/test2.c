#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "kiss.h"

int main(void)
{
	char *space = NULL;
	char *space2 = NULL;
	char *filename = "asdfasdf";
	FILE *fp = NULL;

	INFO("Hello");
	CHECK(space = malloc(1000), "malloc failed");
	CHECK(space2 = realloc(space, 10000), "realloc failed");
	space = space2;
	CHECK(fp = fopen(filename, "r"), "fopen failed: %s", filename);
	CHECK(fclose(fp) == 0, "fclose failed");
	free(space);
	space = NULL;
	INFO("Bye");
	exit(0);
error:
	exit(1);
}
