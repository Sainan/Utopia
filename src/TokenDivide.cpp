#include "TokenDivide.hpp"

namespace Utopia
{
	TokenDivide::TokenDivide(const SourceLocation& loc)
		: Token(TOKEN_DIVIDE, loc)
	{
	}

	std::string TokenDivide::getName() const
	{
		return "divide";
	}
}
