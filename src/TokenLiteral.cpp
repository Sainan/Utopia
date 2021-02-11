#include "TokenLiteral.hpp"

namespace Utopia
{
	TokenLiteral::TokenLiteral(std::string&& literal)
		: Token(TOKEN_LITERAL), literal(std::move(literal))
	{
	}
}
