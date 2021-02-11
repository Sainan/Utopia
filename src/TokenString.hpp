#pragma once

#include "Token.hpp"

namespace Utopia
{
	class TokenString : public Token
	{
	public:
		std::string value;

		explicit TokenString(const SourceLocation& loc, std::string&& value);

		[[nodiscard]] std::string getName() const final;
	};
}
