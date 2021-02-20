#pragma once

#include "Token.hpp"

namespace Utopia
{
	class TokenFunction : public Token
	{
	public:
		std::string contents;

		explicit TokenFunction(const SourceLocation& loc, std::string&& contents);

		[[nodiscard]] std::string getName() const final;
	};
}
