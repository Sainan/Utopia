#pragma once

#include "Token.hpp"

namespace Utopia
{
	class TokenBlock : public Token
	{
	public:
		std::string contents{};

		explicit TokenBlock(const SourceLocation& loc);

		[[nodiscard]] std::string getName() const final;
	};
}
