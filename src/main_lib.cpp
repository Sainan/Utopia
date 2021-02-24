// To consume the library, include <libutopia.h> or <libutopia.hpp>

#include "config.hpp"
#if defined(__clang__) || UTO_MODE == UTO_LIB

#include "Program.hpp"

using namespace Utopia;

#ifdef _WIN32
#define EXPORT extern "C" __declspec(dllexport)
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

EXPORT void Utopia_Program_redirectOutput(void* p, void echo_func(void* p, void* arg, const char* str), void* arg)
{
	((Program*)p)->echo_func = (echo_func_t)echo_func;
	((Program*)p)->echo_func_arg = arg;
}

static void echo_impl_redirectOutputToString(void* p, void* arg, const char* str)
{
	((std::string*)arg)->append(str);
}

EXPORT void Utopia_Program_redirectOutputToString(void* p, void* str)
{
	return Utopia_Program_redirectOutput(p, &echo_impl_redirectOutputToString, str);
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
