#pragma once

#include "Token.hpp"

namespace Utopia
{
	class TokenLiteral : public Token
	{
	public:
		std::string literal;

		explicit TokenLiteral(size_t line_num, std::string&& literal);

		[[nodiscard]] std::string getName() const final;
	};
}
