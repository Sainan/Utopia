#pragma once

#include "Token.hpp"

namespace Utopia
{
	class TokenDivide : public Token
	{
	public:
		explicit TokenDivide(const SourceLocation& loc);

		[[nodiscard]] std::string getName() const final;
	};
}
