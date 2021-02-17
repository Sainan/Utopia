#pragma once

#include "fwddecl.hpp"

#include <memory>
#include <string>
#include <vector>

#include "Data.hpp"
#include "SourceLocation.hpp"

namespace Utopia
{
	using echo_func_t = void(*)(const char*);

	class Program
	{
	public:
		static void echo_impl_stdout(const char*);

		// program information
		std::vector<std::unique_ptr<Data>> variables;
		std::vector<uint8_t> ops;
		std::vector<SourceLocation> op_locs; // "debug information"

		// environment information
		echo_func_t echo_func = &echo_impl_stdout;

	public:
		[[nodiscard]] void fromString(std::string&& name, const std::string& code);
		[[nodiscard]] void fromFile(std::string&& path);

		void execute();
	};
}
