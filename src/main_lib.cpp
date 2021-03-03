// To consume the library, include <libutopia.h> or <libutopia.hpp>

#include "config.hpp"
#if defined(__clang__) || UTO_MODE == UTO_LIB

#include "Program.hpp"
#include "Warning.hpp"

using namespace Utopia;

#ifdef _WIN32
#define EXPORT extern "C" __declspec(dllexport)
#elif defined(__EMSCRIPTEN__)
#include "emscripten.h"
#define EXPORT extern "C" EMSCRIPTEN_KEEPALIVE
#else
#define EXPORT extern "C" __attribute__ ((visibility ("default")))
#endif

EXPORT void* Utopia_string_alloc()
{
	return new std::string();
}

EXPORT const char* Utopia_string_read(void* str)
{
	return ((std::string*)str)->c_str();
}

EXPORT void Utopia_string_free(void* str)
{
	delete (std::string*)str;
}

EXPORT void* Utopia_Program_alloc()
{
	return new Program();
}

EXPORT void Utopia_Program_fromFile(void* p, const char* path)
{
	return ((Program*)p)->fromFile(path);
}

EXPORT void Utopia_Program_fromString(void* p, const char* name, const char* code)
{
	return ((Program*)p)->fromString(name, code);
}

EXPORT void Utopia_Program_redirectOutput(void* p, void echo_func(const char* str, void* arg), void* arg)
{
	((Program*)p)->echo_func = (echo_func_t)echo_func;
	((Program*)p)->echo_func_arg = arg;
}

static void impl_redirectOutputToString(const char* str, void* arg)
{
	((std::string*)arg)->append(str);
}

EXPORT void Utopia_Program_redirectOutputToString(void* p, void* str)
{
	return Utopia_Program_redirectOutput(p, &impl_redirectOutputToString, str);
}

EXPORT void Utopia_Program_redirectWarnings(void* p, void warn_func(const void* warning, void* arg), void* arg)
{
	((Program*)p)->warn_func = (warn_func_t)warn_func;
	((Program*)p)->warn_func_arg = arg;
}

static void impl_redirectWarningsToString(const void* warning, void* arg)
{
	((std::string*)arg)->append(((const Warning*)warning)->toString()).append(1, '\n');
}

EXPORT void Utopia_Program_redirectWarningsToString(void* p, void* str)
{
	return Utopia_Program_redirectWarnings(p, &impl_redirectWarningsToString, str);
}

EXPORT void Utopia_Program_execute(void* p)
{
	return ((Program*)p)->execute();
}

EXPORT void Utopia_Program_free(void* p)
{
	delete (Program*)p;
}

EXPORT void* Utopia_SourceLocation_toString(const void* loc)
{
	return new std::string(((const SourceLocation*)loc)->toString());
}

EXPORT const char* Utopia_SourceLocation_file(const void* loc)
{
	return ((const SourceLocation*)loc)->name.operator std::string().c_str();
}

EXPORT size_t Utopia_SourceLocation_line(const void* loc)
{
	return ((const SourceLocation*)loc)->line;
}

EXPORT size_t Utopia_SourceLocation_character(const void* loc)
{
	return ((const SourceLocation*)loc)->character;
}

EXPORT void* Utopia_Warning_toString(const void* warning)
{
	return new std::string(((const Warning*)warning)->toString());
}

EXPORT const char* Utopia_Warning_what(const void* warning)
{
	return ((const Warning*)warning)->message.c_str();
}

EXPORT const void* Utopia_Warning_where(const void* warning)
{
	return &((const Warning*)warning)->loc;
}

#endif
