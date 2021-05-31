#pragma once

#include "TokenContainer.hpp"

namespace Utopia
{
	class TokenConcat : public TokenContainer
	{
	public:
		explicit TokenConcat(const SourceLocation& loc);
		explicit TokenConcat(TokenContainer&& b);

		[[nodiscard]] DataType getReturnType() const final;
	};
}
