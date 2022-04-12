#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int main(void)
{
	char *space = NULL;
	char *space2 = NULL;
	FILE *fp = NULL;

	fprintf(stderr, "Hello\n");
	space = malloc(1000);
	if (!space)
		goto error;
	space2 = realloc(space, 10000);
	if (!space2)
		goto error;
	space = space2;
	fp = fopen("asdfasdf", "r");
	if (!fp)
		goto error;
	if (fclose(fp) < 0)
		goto error;
	fp = NULL;
	free(space);
	space = NULL;
	fprintf(stderr, "Bye\n");
	exit(0);
error:
	perror("error");
	exit(1);
}
