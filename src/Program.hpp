#pragma once

#include "Scope.hpp"

#include <memory>
#include <string>
#include <vector>

#include "Data.hpp"

namespace Utopia
{
	using echo_func_t = void(*)(const char* str, void* arg);
	using warn_func_t = void(*)(const Warning* warning, void* arg);

	class Program : public Scope
	{
	public:
		static void echo_impl_stdout(const char* str, void* arg);
		static void warn_impl_stderr(const Warning* warning, void* arg);

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
