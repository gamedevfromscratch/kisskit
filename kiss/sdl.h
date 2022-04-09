// Wrapper macros for SDL

#define SDL_STRERROR()			(*SDL_GetError() ? ": " : ""), SDL_GetError()

#define SDL_ERROR(M, ...)		EPRINT(DEBUG_LEVEL("[ERROR] ") DEBUG_LOC_FMT M "%s%s\n" DEBUG_LOC_VALS, ##__VA_ARGS__, SDL_STRERROR())

#define SDL_WARN(M, ...)		EPRINT(DEBUG_LEVEL("[WARN]  ") DEBUG_LOC_FMT M "%s%s\n" DEBUG_LOC_VALS, ##__VA_ARGS__, SDL_STRERROR())

#define SDL_CHECK(A, M, ...)		BEGIN if (!(A)) { SDL_ERROR(M, ##__VA_ARGS__); SDL_ClearError(); goto error; } END

#define SDL_ZERO(func, ...)		({ typeof(func(__VA_ARGS__))rv = func(__VA_ARGS__); SDL_CHECK(rv == 0, #func " failed"); rv; })
#define SDL_CHK(func, ...)		({ typeof(func(__VA_ARGS__))rv = func(__VA_ARGS__); SDL_CHECK(rv != 0, #func " failed"); rv; })
#define SDL_NNEG(func, ...)		({ typeof(func(__VA_ARGS__))rv = func(__VA_ARGS__); SDL_CHECK(rv >= 0, #func " failed"); rv; })

#define SDL_INIT(...)			SDL_ZERO(SDL_Init, __VA_ARGS__), SDL_ClearError()
#define SDL_CREATEWINDOW(...)		SDL_CHK(SDL_CreateWindow, __VA_ARGS__)
#define SDL_CREATERGBSURFACE(...)	SDL_CHK(SDL_CreateRGBSurface, __VA_ARGS__)
#define SDL_GETWINDOWSURFACE(...)	SDL_CHK(SDL_GetWindowSurface, __VA_ARGS__)
#define SDL_CONVERTSURFACE(...)		SDL_CHK(SDL_ConvertSurface, __VA_ARGS__)
#define SDL_BLITSURFACE(...)		SDL_ZERO(SDL_BlitSurface, __VA_ARGS__)
#define SDL_BLITSCALED(...)		SDL_ZERO(SDL_BlitScaled, __VA_ARGS__)
#define SDL_UPDATEWINDOWSURFACE(...)	SDL_ZERO(SDL_UpdateWindowSurface, __VA_ARGS__)
#define SDL_FILLRECT(...)		SDL_ZERO(SDL_FillRect, __VA_ARGS__)
#define SDL_WAITEVENT(...)		SDL_CHK(SDL_WaitEvent, __VA_ARGS__)
#define SDL_WAITEVENTTIMEOUT(...)	SDL_CHK(SDL_WaitEventTimeout, __VA_ARGS__)
#define SDL_CREATERENDERER(...)		SDL_CHK(SDL_CreateRenderer, __VA_ARGS__)
#define SDL_CREATETEXTUREFROMSURFACE(...) SDL_CHK(SDL_CreateTextureFromSurface, __VA_ARGS__)
#define SDL_RENDERCLEAR(...)		SDL_ZERO(SDL_RenderClear, __VA_ARGS__)
#define SDL_RENDERCOPY(...)		SDL_ZERO(SDL_RenderCopy, __VA_ARGS__)
#define SDL_RENDERCOPYEX(...)		SDL_ZERO(SDL_RenderCopyEx, __VA_ARGS__)
#define SDL_RENDERDRAWRECT(...)		SDL_ZERO(SDL_RenderDrawRect, __VA_ARGS__)
#define SDL_RENDERFILLRECT(...)		SDL_ZERO(SDL_RenderFillRect, __VA_ARGS__)
#define SDL_RENDERDRAWLINE(...)		SDL_ZERO(SDL_RenderDrawLine, __VA_ARGS__)
#define SDL_RENDERDRAWPOINT(...)	SDL_ZERO(SDL_RenderDrawPoint, __VA_ARGS__)
#define SDL_RENDERDRAWPOINTS(...)	SDL_ZERO(SDL_RenderDrawPoints, __VA_ARGS__)
#define SDL_RENDERSETVIEWPORT(...)	SDL_ZERO(SDL_RenderSetViewport, __VA_ARGS__)
#define SDL_SETCOLORKEY(...)		SDL_ZERO(SDL_SetColorKey, __VA_ARGS__)
#define SDL_SETTEXTURECOLORMOD(...)	SDL_ZERO(SDL_SetTextureColorMod, __VA_ARGS__)
#define SDL_SETTEXTUREBLENDMODE(...)	SDL_ZERO(SDL_SetTextureBlendMode, __VA_ARGS__)
#define SDL_SETTEXTUREALPHAMOD(...)	SDL_ZERO(SDL_SetTextureAlphaMod, __VA_ARGS__)
#define SDL_QUERYTEXTURE(...)		SDL_ZERO(SDL_QueryTexture, __VA_ARGS__)
#define SDL_DESTROYTEXTURE(ptr)		((ptr) ? (SDL_DestroyTexture(ptr), (ptr) = NULL) : NULL)
#define SDL_DESTROYRENDERER(ptr)	((ptr) ? (SDL_DestroyRenderer(ptr), (ptr) = NULL) : NULL)
#define SDL_DESTROYWINDOW(ptr)		((ptr) ? (SDL_DestroyWindow(ptr), (ptr) = NULL) : NULL)
#define SDL_FREESURFACE(ptr)		(SDL_FreeSurface(ptr), (ptr) = NULL)
#define SDL_SETHINT(name, value)	(SDL_SetHint((name), (value)) ? SDL_TRUE : (WARN("SDL_Hint not set: %s = %s", (name), (value)), SDL_FALSE))
#define SDL_NUMJOYSTICKS(...)		SDL_NNEG(SDL_NumJoysticks, __VA_ARGS__)
#define SDL_JOYSTICKOPEN(...)		SDL_CHK(SDL_JoystickOpen, __VA_ARGS__)
#define SDL_JOYSTICKCLOSE(ptr)		((ptr) ? (SDL_JoystickClose(ptr), (ptr) = NULL) : NULL)

// Wrapper macros for SDL_image

#define IMG_INIT(flags)			({ int rv = IMG_Init(flags); SDL_CHECK((rv & (flags)) == (flags), "IMG_Init failed"); rv; })
#define IMG_LOAD(...)			SDL_CHK(IMG_Load, __VA_ARGS__)
#define IMG_SAVEPNG(...)		SDL_ZERO(IMG_SavePNG, __VA_ARGS__)

// Wrapper macros for SDL_ttf

#define TTF_INIT(...)			SDL_ZERO(TTF_Init, __VA_ARGS__)
#define TTF_OPENFONT(...)		SDL_CHK(TTF_OpenFont, __VA_ARGS__)
#define TTF_OPENFONTINDEX(...)		SDL_CHK(TTF_OpenFontIndex, __VA_ARGS__)
#define TTF_RENDERTEXT_SOLID(...)	SDL_CHK(TTF_RenderText_Solid, __VA_ARGS__)
#define TTF_RENDERTEXT_BLENDED(...)	SDL_CHK(TTF_RenderText_Blended, __VA_ARGS__)

// Wrapper macros for SDL_mixer

#define MIX_INIT(flags)			({ int rv = Mix_Init(flags); SDL_CHECK((rv & (flags)) == (flags), "Mix_Init failed"); rv; })
#define MIX_OPENAUDIO(...)		SDL_ZERO(Mix_OpenAudio, __VA_ARGS__)
#define MIX_LOADMUS(...)		SDL_CHK(Mix_LoadMUS, __VA_ARGS__)
#define MIX_LOADWAV(...)		SDL_CHK(Mix_LoadWAV, __VA_ARGS__)
#define MIX_FREEMUSIC(ptr)		((ptr) ? (Mix_FreeMusic(ptr), (ptr) = NULL) : NULL)
#define MIX_FREECHUNK(ptr)		((ptr) ? (Mix_FreeChunk(ptr), (ptr) = NULL) : NULL)
#define MIX_PLAYCHANNEL(...)		SDL_NNEG(Mix_PlayChannel, __VA_ARGS__)
#define MIX_PLAYMUSIC(...)		SDL_ZERO(Mix_PlayMusic, __VA_ARGS__)

// Wrapper macros for SDL_opengl

#define SDL_GL_SETATTRIBUTE(...)	SDL_ZERO(SDL_GL_SetAttribute, __VA_ARGS__)
#define SDL_GL_CREATECONTEXT(...)	SDL_CHK(SDL_GL_CreateContext, __VA_ARGS__)
#define SDL_GL_SETSWAPINTERVAL(...)	SDL_ZERO(SDL_GL_SetSwapInterval, __VA_ARGS__)
