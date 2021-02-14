#include "TokenPlus.hpp"

#include "DataInt.hpp"
#include "TokenInt.hpp"

namespace Utopia
{
	TokenPlus::TokenPlus(const SourceLocation& loc)
		: TokenArithmetic(TOKEN_PLUS, loc, &arithmetic_plus)
	{
	}
}
