#include "TokenLiteral.hpp"

namespace Utopia
{
	TokenLiteral::TokenLiteral(const SourceLocation& loc, std::string&& literal)
		: Token(TOKEN_LITERAL, loc), literal(std::move(literal))
	{
	}

	std::string TokenLiteral::getName() const
	{
		return std::string("literal '").append(literal).append(1, '\'');
	}
}
