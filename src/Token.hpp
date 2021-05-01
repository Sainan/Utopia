#pragma once

#include <cstdint>
#include <string>

#include "SourceLocation.hpp"

namespace Utopia
{
	enum TokenType : uint8_t
	{
		TOKEN_STRING,
		TOKEN_INT,
		TOKEN_FUNC,
		_TOKEN_RVALUE_END,

		TOKEN_LITERAL = _TOKEN_RVALUE_END,
		TOKEN_BLOCK,

		_TOKEN_CONTAINER_BEGIN,
		TOKEN_PLUS = _TOKEN_CONTAINER_BEGIN,
		TOKEN_MINUS,
		TOKEN_MULTIPLY,
		TOKEN_DIVIDE,
		TOKEN_ASSIGNMENT,
		TOKEN_CONCAT,
		TOKEN_EQUALS,
		_TOKEN_CONTAINER_END,
		_TOKEN_NUM = _TOKEN_CONTAINER_END
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

		[[nodiscard]] virtual bool isRValue() const;

		[[nodiscard]] bool isContainer() const;

		[[nodiscard]] static const char* getTypeName(const TokenType type);
		[[nodiscard]] virtual std::string getName() const;

		[[nodiscard]] virtual const SourceLocation& getLeftmostSourceLocation() const;

		[[noreturn]] void throwUnexpected() const;
		void expectType(TokenType expected_type) const;
	};
}
