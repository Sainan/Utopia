#pragma once

#include "Token.hpp"

namespace Utopia
{
	class TokenMultiply : public Token
	{
	public:
		explicit TokenMultiply(const SourceLocation& loc);

		[[nodiscard]] std::string getName() const final;
	};
}
