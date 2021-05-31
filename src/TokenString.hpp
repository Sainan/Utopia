#pragma once

#include "Token.hpp"

namespace Utopia
{
	class TokenString : public Token
	{
	public:
		std::string value{};

		explicit TokenString(const SourceLocation& loc);

		[[nodiscard]] std::string getName() const final;

		[[nodiscard]] DataType getReturnType() const final;
	};
}
