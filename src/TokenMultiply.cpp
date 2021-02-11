#include "TokenMultiply.hpp"

namespace Utopia
{
	TokenMultiply::TokenMultiply(size_t line_num)
		: Token(TOKEN_MULTIPLY, line_num)
	{
	}

	std::string TokenMultiply::getName() const
	{
		return "multiply";
	}
}
