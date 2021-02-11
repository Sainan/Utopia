#pragma once

#include "Token.hpp"

namespace Utopia
{
	class TokenPlus : public Token
	{
	public:
		explicit TokenPlus(size_t line_num);

		[[nodiscard]] std::string getName() const final;
	};
}
