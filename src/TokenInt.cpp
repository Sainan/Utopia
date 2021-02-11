#include "TokenInt.hpp"

namespace Utopia
{
	TokenInt::TokenInt(const SourceLocation& loc, long long value)
		: Token(TOKEN_INT, loc), value(value)
	{
	}

	std::string TokenInt::getName() const
	{
		return std::string("integer '").append(std::to_string(value)).append(1, '\'');
	}
}
