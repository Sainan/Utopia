#pragma once

#include "fwddecl.hpp"

#include <memory>
#include <string>
#include <vector>

#include "Data.hpp"
#include "Op.hpp"
#include "Shared.hpp"

namespace Utopia
{
	class Program
	{
	public:
		std::vector<std::unique_ptr<Data>> variables;
		std::vector<uint8_t> ops;

	public:
		[[nodiscard]] static Program fromString(std::string&& name, const std::string& code);
		[[nodiscard]] static Program fromFile(std::string&& path);

		void execute() const;
	};
}
