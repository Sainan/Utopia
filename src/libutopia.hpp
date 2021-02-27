#pragma once

#include <string>

extern "C"
{
	#include "libutopia.h"
}

namespace Utopia
{
	template <typename T>
	T freecopy(T* tmp)
	{
		T cpy(std::move(*tmp));
		delete tmp;
		return cpy;
	}

	class SourceLocation
	{
	private:
		const void* const loc;

	public:
		explicit SourceLocation(const void* loc)
			: loc(loc)
		{
		}

		[[nodiscard]] std::string toString()
		{
			return freecopy((std::string*)Utopia_SourceLocation_toString(loc));
		}

		[[nodiscard]] const char* file()
		{
			return Utopia_SourceLocation_file(loc);
		}

		[[nodiscard]] size_t line()
		{
			return Utopia_SourceLocation_line(loc);
		}

		[[nodiscard]] size_t character()
		{
			return Utopia_SourceLocation_character(loc);
		}
	};

	class Warning
	{
	private:
		const void* const warning;

	public:
		[[nodiscard]] std::string toString()
		{
			return freecopy((std::string*)Utopia_Warning_toString(warning));
		}

		[[nodiscard]] const char* what()
		{
			return Utopia_Warning_what(warning);
		}

		[[nodiscard]] SourceLocation where()
		{
			return SourceLocation(Utopia_Warning_where(warning));
		}
	};
	static_assert(sizeof(Warning) == sizeof(void*));

	class Program
	{
	private:
		void* const p;

	public:
		explicit Program()
			: p(Utopia_Program_alloc())
		{
		}

		~Program()
		{
			Utopia_Program_free(p);
		}

		void fromFile(const char* path)
		{
			return Utopia_Program_fromFile(p, path);
		}

		void fromString(const char* name, const char* code)
		{
			return Utopia_Program_fromString(p, name, code);
		}

		void redirectOutput(void echo_func(const char* str, void* arg), void* arg = nullptr)
		{
			return Utopia_Program_redirectOutput(this, echo_func, arg);
		}

		void redirectOutputToString(std::string* str)
		{
			return Utopia_Program_redirectOutputToString(this, str);
		}

		void redirectWarnings(void warn_func(Warning warning, void* arg), void* arg = nullptr)
		{
			return Utopia_Program_redirectWarnings(p, (void(*)(const void*, void*))warn_func, arg);
		}

		void redirectWarningsToString(std::string* str)
		{
			return Utopia_Program_redirectWarningsToString(p, str);
		}

		void execute()
		{
			return Utopia_Program_execute(p);
		}
	};
}
