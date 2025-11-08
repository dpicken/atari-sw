#ifndef util_Debug_h
#define util_Debug_h

#if !defined(NDEBUG)
#define DEBUG_ONLY(x) x
#else
//#define DEBUG_ONLY(x) x
#define DEBUG_ONLY(x) static_assert(true)
#endif

#endif // ifndef util_Debug_h
