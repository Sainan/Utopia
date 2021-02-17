// To consume the library, include <libutopia.h> or <libutopia.hpp>

#include "config.hpp"
#if defined(__clang__) || UTO_MODE == UTO_LIB

#include "Program.hpp"

using namespace Utopia;

#ifdef _WIN32
#define EXPORT extern "C" __declspec(dllexport)
#else
#define EXPORT extern "C"
#endif

EXPORT void Utopia_execute_file(const char* path)
{
	Program::fromFile(path).execute();
}

EXPORT void Utopia_execute_string(const char* name, const char* code)
{
	Program::fromString(name, code).execute();
}

#endif
