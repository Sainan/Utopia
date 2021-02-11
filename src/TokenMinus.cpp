#include "TokenMinus.hpp"

namespace Utopia
{
	TokenMinus::TokenMinus(const SourceLocation& loc)
		: Token(TOKEN_MINUS, loc)
	{
	}

	std::string TokenMinus::getName() const
	{
		return "minus";
	}
}
