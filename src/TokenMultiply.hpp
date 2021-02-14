#pragma once

#include "TokenArithmetic.hpp"

namespace Utopia
{
	class TokenMultiply : public TokenArithmetic
	{
	public:
		explicit TokenMultiply(const SourceLocation& loc);
	};
}
