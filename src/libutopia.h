#pragma once

#ifdef _WIN32
#define IMPORT __declspec(dllimport)
#else
#define IMPORT
#endif

IMPORT void* Utopia_Program_fromFile(const char* path);
IMPORT void* Utopia_Program_fromString(const char* name, const char* code);
IMPORT void Utopia_Program_redirectOutput(void* p, void echo_func(const char*));
IMPORT void Utopia_Program_execute(void* p);
IMPORT void Utopia_Program_free(void* p);
