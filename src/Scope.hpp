#pragma once

#include "fwddecl.hpp"

#include <cstdint>
#include <vector>

#include "SourceLocation.hpp"

namespace Utopia
{
	struct Scope
	{
		std::vector<uint8_t> ops = {};
		std::vector<SourceLocation> op_locs = {}; // "debug information"

		void execute(Program* p) const;
	};
}
