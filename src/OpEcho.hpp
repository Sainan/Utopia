#pragma once

#include "Op.hpp"

namespace Utopia
{
	class OpEcho : public Op
	{
	public:
		size_t getNumArgs() const;
		void execute(const Program& p, std::vector<uint8_t>&& args) const;
	};
}
