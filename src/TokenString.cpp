#include "TokenString.hpp"

#include "escape_string.hpp"

namespace Utopia
{
	TokenString::TokenString(const SourceLocation& loc)
		: Token(TOKEN_STRING, loc)
	{
	}

	std::string TokenString::getName() const
	{
		return std::string("string \"").append(escape_string(value)).append(1, '"');
	}
}
