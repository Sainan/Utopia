#pragma once

#include "Token.hpp"

namespace Utopia
{
	class TokenBool : public Token
	{
	public:
		bool value;

		explicit TokenBool(const SourceLocation& loc, bool value);

		[[nodiscard]] std::string getName() const final;

		[[nodiscard]] DataType getReturnType() const final;
	};
}
