#include "TokenMultiply.hpp"

namespace Utopia
{
	TokenMultiply::TokenMultiply(const SourceLocation& loc)
		: Token(TOKEN_MULTIPLY, loc)
	{
	}

	std::string TokenMultiply::getName() const
	{
		return "multiply";
	}
}
