#pragma once

#include "Token.hpp"

namespace Utopia
{
	class TokenInt : public Token
	{
	public:
		long long value;

		explicit TokenInt(size_t line_num, long long value);

		[[nodiscard]] std::string getName() const final;
	};
}
