#include "TokenMultiply.hpp"

namespace Utopia
{
	TokenMultiply::TokenMultiply(const SourceLocation& loc)
		: TokenArithmetic(TOKEN_MULTIPLY, loc, &arithmetic_multiply)
	{
	}
}
