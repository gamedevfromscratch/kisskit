#define UNUSED(x)		(void)(x)
#define NELEMS(x)		(sizeof(x) / sizeof((x)[0]))

#define PI			3.14159265358979323846
#define RAD(a)			((a) * PI / 180)
#define DEG(a)			((a) * 180 / PI)

// typedef char bit;

// Largely inspired by "Zed's Awesome Debug Macros"
// From http://c.learncodethehardway.org/book/ex20.html

#define BEGIN			do {
#define END			} while (0)

#define EPRINT(...)		fprintf(stderr, __VA_ARGS__)

#ifdef NDEBUG
#define DEBUG_LOC_FMT
#define DEBUG_LOC_VALS
#define DEBUG_LEVEL(X)
#define DEBUG(M, ...)
#else
#define DEBUG_LOC_FMT		"%s:%d %s: "
#define DEBUG_LOC_VALS		, __FILE__, __LINE__, __func__
#define DEBUG_LEVEL(X)		X
#define DEBUG(M, ...)		EPRINT(DEBUG_LEVEL("[DEBUG ]") DEBUG_LOC_FMT M "\n" DEBUG_LOC_VALS, ##__VA_ARGS__)
#endif

#define STRERROR()		(errno == 0 ? "" : ": "), (errno == 0 ? "" : strerror(errno))

// TODO which of these are needed?

#define ERROR1(M, ...)		EPRINT(DEBUG_LEVEL("[ERROR] ") DEBUG_LOC_FMT M "\n" DEBUG_LOC_VALS, ##__VA_ARGS__)
#define WARN1(M, ...)		EPRINT(DEBUG_LEVEL("[WARN]  ") DEBUG_LOC_FMT M "\n" DEBUG_LOC_VALS, ##__VA_ARGS__)
#define CHECK1(A, M, ...)	BEGIN if (!(A)) { ERROR1(M, ##__VA_ARGS__); goto error; } END

#define CERROR(M, ...)		EPRINT(DEBUG_LEVEL("[ERROR] ") DEBUG_LOC_FMT M "%s%s\n" DEBUG_LOC_VALS, ##__VA_ARGS__, STRERROR())
#define WARN(M, ...)		EPRINT(DEBUG_LEVEL("[WARN]  ") DEBUG_LOC_FMT M "%s%s\n" DEBUG_LOC_VALS, ##__VA_ARGS__, STRERROR())
#define INFO(M, ...)		EPRINT(DEBUG_LEVEL("[INFO]  ") DEBUG_LOC_FMT M "\n" DEBUG_LOC_VALS, ##__VA_ARGS__)

#define CHECK(A, M, ...)	BEGIN if (!(A)) { CERROR(M, ##__VA_ARGS__); errno = 0; goto error; } END

//#define SENTINEL(M, ...)	BEGIN CERROR(M, ##__VA_ARGS__); errno = 0; goto error; END

//#define CHECK_DEBUG(A, M, ...)	if(!(A)) { debug(M, ##__VA_ARGS__); errno = 0; goto error; }

#define TODO(M, ...)		EPRINT(DEBUG_LEVEL("[TODO]  ") DEBUG_LOC_FMT M "\n" DEBUG_LOC_VALS, ##__VA_ARGS__)
#define FIXME(M, ...)		EPRINT(DEBUG_LEVEL("[FIXME] ") DEBUG_LOC_FMT M "\n" DEBUG_LOC_VALS, ##__VA_ARGS__)
#define XXX(M, ...)		EPRINT(DEBUG_LEVEL("[XXX]   ") DEBUG_LOC_FMT M "\n" DEBUG_LOC_VALS, ##__VA_ARGS__)
