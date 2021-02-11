#pragma once

#include "Token.hpp"

namespace Utopia
{
	class TokenInt : public Token
	{
	public:
		long long value;

		explicit TokenInt(const SourceLocation& loc, long long value);

		[[nodiscard]] std::string getName() const final;
	};
}
