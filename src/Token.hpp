#pragma once

#include <cstdint>
#include <string>

#include "SourceLocation.hpp"

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
		TOKEN_ASSIGNMENT,
	};

	class Token
	{
	public:
		const TokenType type;
		const SourceLocation loc;

	protected:
		explicit Token(TokenType type, const SourceLocation& loc);

	public:
		virtual ~Token();

		[[nodiscard]] virtual std::string getName() const = 0;

		void throwUnexpected() const;
		void expectType(TokenType expected_type) const;
	};
}
