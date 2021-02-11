#pragma once

#include "Token.hpp"

#include <string>

namespace Utopia
{
	class TokenLiteral : public Token
	{
	public:
		std::string literal;

		TokenLiteral(std::string&& literal);
	};
}
