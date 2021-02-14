#include "TokenDivide.hpp"

namespace Utopia
{
	TokenDivide::TokenDivide(const SourceLocation& loc)
		: TokenArithmetic(TOKEN_DIVIDE, loc, &arithmetic_divide)
	{
	}
}
