#pragma once

#include <cstdint>
#include <string>

namespace Utopia
{
	enum TokenType : uint8_t
	{
		TOKEN_LITERAL,
		TOKEN_STRING,
		TOKEN_INT,
		TOKEN_PLUS,
		TOKEN_MINUS,
		TOKEN_MULTIPLY,
		TOKEN_DIVIDE,
	};

	class Token
	{
	public:
		const TokenType type;
		const size_t line_num;

	protected:
		explicit Token(TokenType type, size_t line_num);

	public:
		virtual ~Token();

		[[nodiscard]] virtual std::string getName() const = 0;

		void throwUnexpected() const;
		void expectType(TokenType expected_type) const;
	};
}
