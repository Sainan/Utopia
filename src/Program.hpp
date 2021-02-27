#pragma once

#include "Scope.hpp"

#include <memory>
#include <string>
#include <vector>

#include "Data.hpp"

namespace Utopia
{
	using echo_func_t = void(*)(Program* p, void* arg, const char* str);
	using warn_func_t = void(*)(Program* p, void* arg, const Warning* warning);

	class Program : public Scope
	{
	public:
		static void echo_impl_stdout(Program* p, void* arg, const char* str);
		static void warn_impl_stderr(Program* p, void* arg, const Warning* warning);

		std::vector<std::unique_ptr<Data>> variables;

		// environment information
		echo_func_t echo_func = &echo_impl_stdout;
		void* echo_func_arg = nullptr;

		warn_func_t warn_func = &warn_impl_stderr;
		void* warn_func_arg = nullptr;

	public:
		void printVariables();

		void fromString(std::string&& name, const std::string& code);
		void fromFile(std::string&& path);

		void eraseDebugInformation(); // Kind of useless at the moment because the compiler doesn't have the means to persist debug information

		void execute();
	};
}
