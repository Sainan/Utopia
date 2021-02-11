#pragma once

#include <cstdint>

namespace Utopia
{
	enum TokenType : uint8_t
	{
		TOKEN_LITERAL,
		TOKEN_STRING,
	};

	class Token
	{
	public:
		const TokenType type;

	protected:
		explicit Token(const TokenType type)
			: type(type)
		{
		}
	};
}
