#pragma once

#ifdef _WIN32
#define IMPORT __declspec(dllimport)
#else
#define IMPORT
#endif

IMPORT void* Utopia_string_alloc(); // allocates a new std::string to be used as `void* str` -- don't forget to free it!
IMPORT const char* Utopia_string_read(void* str);
IMPORT void Utopia_string_free(void* str);

IMPORT void* Utopia_Program_alloc(); // allocates a new Program to be used as `void* p` -- don't forget to free it!
IMPORT void Utopia_Program_fromFile(void* p, const char* path);
IMPORT void Utopia_Program_fromString(void* p, const char* name, const char* code);
IMPORT void Utopia_Program_redirectOutput(void* p, void echo_func(const char* str, void* arg), void* arg);
IMPORT void Utopia_Program_redirectOutputToString(void* p, void* str);
IMPORT void Utopia_Program_redirectWarnings(void* p, void warn_func(const void* warning, void* arg), void* arg);
IMPORT void Utopia_Program_redirectWarningsToString(void* p, void* str);
IMPORT void Utopia_Program_execute(void* p);
IMPORT void Utopia_Program_free(void* p);

IMPORT void* Utopia_SourceLocation_toString(const void* loc); // allocates a new std::string to be used as `void* str` -- don't forget to free it!
IMPORT const char* Utopia_SourceLocation_file(const void* loc);
IMPORT size_t Utopia_SourceLocation_line(const void* loc);
IMPORT size_t Utopia_SourceLocation_character(const void* loc);

IMPORT void* Utopia_Warning_toString(const void* warning); // allocates a new std::string to be used as `void* str` -- don't forget to free it!
IMPORT const char* Utopia_Warning_what(const void* warning);
IMPORT const void* Utopia_Warning_where(const void* warning); // returns a SourceLocation pointer
