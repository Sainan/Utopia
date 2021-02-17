#pragma once

#ifdef _WIN32
#define IMPORT __declspec(dllimport)
#else
#define IMPORT
#endif

IMPORT void Utopia_execute_file(const char* path);
IMPORT void Utopia_execute_string(const char* name, const char* code);
