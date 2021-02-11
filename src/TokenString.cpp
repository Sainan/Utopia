#include "TokenString.hpp"

namespace Utopia
{
	TokenString::TokenString(std::string&& value)
		: Token(TOKEN_STRING), value(std::move(value))
	{
	}
}
