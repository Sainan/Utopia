#include "TokenArithmetic.hpp"

namespace Utopia
{
	TokenArithmetic::TokenArithmetic(const TokenType type, const SourceLocation& loc)
		: TokenContainer(type, loc)
	{
	}

	DataType TokenArithmetic::getReturnType() const
	{
		return DATA_INT;
	}
}
