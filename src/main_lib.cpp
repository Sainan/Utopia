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

EXPORT void* Utopia_Program_fromFile(const char* path)
{
	auto p = new Program();
	p->fromFile(path);
	return p;
}

EXPORT void* Utopia_Program_fromString(const char* name, const char* code)
{
	auto p = new Program();
	p->fromString(name, code);
	return p;
}

EXPORT void Utopia_Program_redirectOutput(void* p, void echo_func(const char*))
{
	((Program*)p)->echo_func = echo_func;
}

EXPORT void Utopia_Program_execute(void* p)
{
	return ((Program*)p)->execute();
}

EXPORT void Utopia_Program_free(void* p)
{
	delete (Program*)p;
}

#endif
