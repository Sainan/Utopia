#pragma once

#ifdef _WIN32
#define IMPORT __declspec(dllimport)
#else
#define IMPORT
#endif

IMPORT void* Utopia_string_alloc(); // allocates a new std::string to be used as `void* str` -- don't forget to free it!
IMPORT const char* Utopia_string_read(void* str);
IMPORT void Utopia_string_free(void* str);

IMPORT void* Utopia_Program_fromFile(const char* path); // allocates a new Program to be used as `void* p` -- don't forget to free it!
IMPORT void* Utopia_Program_fromString(const char* name, const char* code); // allocates a new Program to be used as `void* p` -- don't forget to free it!
IMPORT void Utopia_Program_redirectOutput(void* p, void echo_func(void* p, void* arg, const char* str), void* arg);
IMPORT void Utopia_Program_redirectOutputToString(void* p, void* str);
IMPORT void Utopia_Program_execute(void* p);
IMPORT void Utopia_Program_free(void* p);
