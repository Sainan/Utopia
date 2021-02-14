#pragma once

#include "TokenArithmetic.hpp"

namespace Utopia
{
	class TokenMinus : public TokenArithmetic
	{
	public:
		explicit TokenMinus(const SourceLocation& loc);
	};
}
