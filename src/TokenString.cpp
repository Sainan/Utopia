#include "TokenString.hpp"

#include "escape_string.hpp"

namespace Utopia
{
	TokenString::TokenString(size_t line_num, std::string&& value)
		: Token(TOKEN_STRING, line_num), value(std::move(value))
	{
	}

	std::string TokenString::getName() const
	{
		return std::string("string \"").append(escape_string(value)).append(1, '"');
	}
}
