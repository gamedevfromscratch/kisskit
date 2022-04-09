// Wrapper macros for libc

#define MALLOC(...)		CHK(malloc, __VA_ARGS__)
#define REALLOC(...)		CHK(realloc, __VA_ARGS__)
#define FREE(ptr)		(free(ptr), ptr = NULL)

#define FOPEN(path, mode)	({ char *p = path; FILE *rv = fopen(p, mode); CHECK(rv, "fopen failed: %s", p); rv; })
#define FCLOSE(fp)		(ZERO(fclose, fp), fp = NULL)
