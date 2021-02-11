#include "TokenMinus.hpp"

namespace Utopia
{
	TokenMinus::TokenMinus(size_t line_num)
		: Token(TOKEN_MINUS, line_num)
	{
	}

	std::string TokenMinus::getName() const
	{
		return "minus";
	}
}
