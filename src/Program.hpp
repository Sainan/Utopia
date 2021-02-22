#pragma once

#include "Scope.hpp"

#include <memory>
#include <string>
#include <vector>

#include "Data.hpp"

namespace Utopia
{
	using echo_func_t = void(*)(const char*);

	class Program : public Scope
	{
	public:
		static void echo_impl_stdout(const char*);

		std::vector<std::unique_ptr<Data>> variables;

		// environment information
		echo_func_t echo_func = &echo_impl_stdout;

	public:
		void printVariables();

		void fromString(std::string&& name, const std::string& code);
		void fromFile(std::string&& path);

		void eraseDebugInformation(); // Kind of useless at the moment because the compiler doesn't have the means to persist debug information

		void execute();
	};
}
