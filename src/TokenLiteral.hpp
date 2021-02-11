#pragma once

#include "Token.hpp"

namespace Utopia
{
	class TokenLiteral : public Token
	{
	public:
		std::string literal;

		explicit TokenLiteral(const SourceLocation& loc, std::string&& literal);

		[[nodiscard]] std::string getName() const final;
	};
}
