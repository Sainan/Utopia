#pragma once

#include "fwddecl.hpp"

#include <memory>
#include <string>
#include <vector>

#include "Data.hpp"
#include "SourceLocation.hpp"

namespace Utopia
{
	class Program
	{
	public:
		std::vector<std::unique_ptr<Data>> variables;
		std::vector<uint8_t> ops;
		std::vector<SourceLocation> op_locs; // "debug information"

	public:
		[[nodiscard]] static Program fromString(std::string&& name, const std::string& code);
		[[nodiscard]] static Program fromFile(std::string&& path);

		void execute();
	};
}
