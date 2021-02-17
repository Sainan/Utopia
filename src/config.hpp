#pragma once

#define UTO_MODE /* If I'm not using the provided build scripts, I'd like to compile: */ UTO_INTERPRETER

// Your options are:
#define UTO_INTERPRETER 0
#define UTO_COMPILER 1
#define UTO_LIB 2
#define UTO_TEST 3
#define UTO_BENCHMARK 4

// That's it!

// Automatically switch to library mode based on visual studio's "configuration type"
#if defined(_WINDLL) || defined(_WINLIB)
#pragma warning(push)
#pragma warning(disable: 4005)
#define UTO_MODE UTO_LIB
#pragma warning(pop)
#endif

// Development flags

#define PERF_PROFILING false
#define DEBUG_TOKENS false
#define DEBUG_VM false
