#pragma once

#include "TokenContainer.hpp"

namespace Utopia
{
	class TokenArithmetic : public TokenContainer
	{
	protected:
		explicit TokenArithmetic(const TokenType type, const SourceLocation& loc);

		[[nodiscard]] DataType getReturnType() const final;
	};
}
