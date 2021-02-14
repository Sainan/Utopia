#pragma once

#include "TokenArithmetic.hpp"

namespace Utopia
{
	class TokenPlus : public TokenArithmetic
	{
	public:
		explicit TokenPlus(const SourceLocation& loc);
	};
}
