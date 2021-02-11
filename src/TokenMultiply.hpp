#pragma once

#include "Token.hpp"

namespace Utopia
{
	class TokenMultiply : public Token
	{
	public:
		explicit TokenMultiply(size_t line_num);

		[[nodiscard]] std::string getName() const final;
	};
}
