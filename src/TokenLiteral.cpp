#include "TokenLiteral.hpp"

namespace Utopia
{
	TokenLiteral::TokenLiteral(size_t line_num, std::string&& literal)
		: Token(TOKEN_LITERAL, line_num), literal(std::move(literal))
	{
	}

	std::string TokenLiteral::getName() const
	{
		return std::string("literal '").append(literal).append(1, '\'');
	}
}
