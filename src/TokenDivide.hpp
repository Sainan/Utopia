#pragma once

#include "Token.hpp"

namespace Utopia
{
	class TokenDivide : public Token
	{
	public:
		explicit TokenDivide(size_t line_num);

		[[nodiscard]] std::string getName() const final;
	};
}
