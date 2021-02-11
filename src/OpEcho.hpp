#pragma once

#include "Op.hpp"

namespace Utopia
{
	class OpEcho : public Op
	{
	public:
		[[nodiscard]] size_t getNumArgs() const final;
		void execute(const Program& p, std::vector<uint8_t>&& args) const final;
	};
}
