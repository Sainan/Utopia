#pragma once

#include "TokenArithmetic.hpp"

namespace Utopia
{
	class TokenDivide : public TokenArithmetic
	{
	public:
		explicit TokenDivide(const SourceLocation& loc);
	};
}
