#include "TokenString.hpp"

#include "escape_string.hpp"

namespace Utopia
{
	TokenString::TokenString(const SourceLocation& loc, std::string&& value)
		: Token(TOKEN_STRING, loc), value(std::move(value))
	{
	}

	std::string TokenString::getName() const
	{
		return std::string("string \"").append(escape_string(value)).append(1, '"');
	}
}
