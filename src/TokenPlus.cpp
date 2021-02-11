#include "TokenPlus.hpp"

namespace Utopia
{
	TokenPlus::TokenPlus(const SourceLocation& loc)
		: Token(TOKEN_PLUS, loc)
	{
	}

	std::string TokenPlus::getName() const
	{
		return "plus";
	}
}
