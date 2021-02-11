#pragma once

#include "fwddecl.hpp"

#include <memory>
#include <string>
#include <vector>

#include "Data.hpp"
#include "Op.hpp"

namespace Utopia
{
	class Program
	{
	public:
		std::vector<std::unique_ptr<Data>> data;
		std::vector<uint8_t> ops;

		[[nodiscard]] static Program fromString(const std::string& code);
		[[nodiscard]] static Program fromFile(const std::string& path);

		void execute() const;
	};
}
