// Wrapper macros for OpenGL

#define GL_STRERROR(err)	((err) == GL_NO_ERROR ? "" : ": "), ((err) == GL_NO_ERROR ? "" : (char *)gluErrorString(err))
#define GL_ERROR(err, M, ...)	EPRINT(DEBUG_LEVEL("[ERROR] ") DEBUG_LOC_FMT M "%s%s\n" DEBUG_LOC_VALS, ##__VA_ARGS__, GL_STRERROR(err))
#define GL_WARN(err, M, ...)	EPRINT(DEBUG_LEVEL("[WARN]  ") DEBUG_LOC_FMT M "%s%s\n" DEBUG_LOC_VALS, ##__VA_ARGS__, GL_STRERROR(err))
#define GL_CHECK(M, ...)	BEGIN GLenum err = glGetError(); if (err != GL_NO_ERROR) { GL_ERROR(err, M, ##__VA_ARGS__); goto error; } END

// Wrapper macros for GLEW

#define GLEW_STRERROR(err)	((err) == GLEW_OK ? "" : ": "), ((err) == GLEW_OK ? "" : (char *)glewGetErrorString(err))
#define GLEW_ERROR(err, M, ...)	EPRINT(DEBUG_LEVEL("[ERROR] ") DEBUG_LOC_FMT M "%s%s\n" DEBUG_LOC_VALS, ##__VA_ARGS__, GLEW_STRERROR(err))
#define GLEW_WARN(err, M, ...)	EPRINT(DEBUG_LEVEL("[WARN]  ") DEBUG_LOC_FMT M "%s%s\n" DEBUG_LOC_VALS, ##__VA_ARGS__, GLEW_STRERROR(err))

#define GLEW_CHECK(err, M, ...)	BEGIN if (err != GLEW_OK) { GLEW_ERROR(err, M, ##__VA_ARGS__); goto error; } END

#define GLEW_CHK(func, ...)	({ typeof(func(__VA_ARGS__))rv = func(__VA_ARGS__); GLEW_CHECK(rv, #func " failed"); rv; })

#define GLEWINIT(...)		GLEW_CHK(glewInit, __VA_ARGS__)
