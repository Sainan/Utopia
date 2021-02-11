#include "TokenInt.hpp"

namespace Utopia
{
	TokenInt::TokenInt(size_t line_num, long long value)
		: Token(TOKEN_INT, line_num), value(value)
	{
	}

	std::string TokenInt::getName() const
	{
		return std::string("integer '").append(std::to_string(value)).append(1, '\'');
	}
}
