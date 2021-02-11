#pragma once

#include "Token.hpp"

namespace Utopia
{
	class TokenPlus : public Token
	{
	public:
		explicit TokenPlus(const SourceLocation& loc);

		[[nodiscard]] std::string getName() const final;
	};
}
