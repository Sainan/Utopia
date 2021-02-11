#include "TokenDivide.hpp"

namespace Utopia
{
	TokenDivide::TokenDivide(size_t line_num)
		: Token(TOKEN_DIVIDE, line_num)
	{
	}

	std::string TokenDivide::getName() const
	{
		return "divide";
	}
}
