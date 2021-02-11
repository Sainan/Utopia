#pragma once

#include "Token.hpp"

namespace Utopia
{
	class TokenMinus : public Token
	{
	public:
		explicit TokenMinus(const SourceLocation& loc);

		[[nodiscard]] std::string getName() const final;
	};
}
