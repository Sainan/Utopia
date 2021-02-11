#pragma once

#include "Token.hpp"

namespace Utopia
{
	class TokenMinus : public Token
	{
	public:
		explicit TokenMinus(size_t line_num);

		[[nodiscard]] std::string getName() const final;
	};
}
