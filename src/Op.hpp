#pragma once

#include "fwddecl.hpp"

#include <cstdint>
#include <vector>

namespace Utopia
{
	class Op
	{
	public:
		virtual size_t getNumArgs() const = 0;
		virtual void execute(const Program& p, std::vector<uint8_t>&& args) const = 0;
	};
}
