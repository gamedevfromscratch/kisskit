#ifdef _WIN32
#define EPRINT(...)		({ int kiss_rv = fprintf(stderr, __VA_ARGS__); fflush(stderr); kiss_rv; })
#endif

// Expression check macros

#define ZERO(func, ...)		({ typeof(func(__VA_ARGS__))kiss_rv = func(__VA_ARGS__); CHECK(kiss_rv == 0, #func " failed"); kiss_rv; })
#define CHK(func, ...)		({ typeof(func(__VA_ARGS__))kiss_rv = func(__VA_ARGS__); CHECK(kiss_rv != 0, #func " failed"); kiss_rv; })
#define NNEG(func, ...)		({ typeof(func(__VA_ARGS__))kiss_rv = func(__VA_ARGS__); CHECK(kiss_rv >= 0, #func " failed"); kiss_rv; })
