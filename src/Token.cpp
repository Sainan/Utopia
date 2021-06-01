#include "Token.hpp"

#include "ParseError.hpp"

namespace Utopia
{
	Token::Token(TokenType type, const SourceLocation& loc)
		: type(type), loc(loc)
	{
	}

	Token::~Token()
	{
	}

	bool Token::isContainer() const
	{
		return type >= _TOKEN_CONTAINER_BEGIN && type < _TOKEN_CONTAINER_END;
	}

	const char* Token::getTypeName(const TokenType type)
	{
		switch (type)
		{
		case TOKEN_STRING:
			return "string";

		case TOKEN_INT:
			return "integer";

		case TOKEN_LITERAL:
			return "literal";

		case TOKEN_BLOCK:
			return "block";

		case TOKEN_FUNC:
			return "function";

		case TOKEN_PLUS:
			return "addition";

		case TOKEN_MINUS:
			return "subtration";

		case TOKEN_MULTIPLY:
			return "multiplication";

		case TOKEN_DIVIDE:
			return "division";

		case TOKEN_ASSIGNMENT:
			return "assignment";

		case TOKEN_CONCAT:
			return "concatenation";
			
		case TOKEN_EQUALS:
			return "is_equal";

		case TOKEN_UNEQUAL:
			return "is_not_equal";

		case _TOKEN_CONTAINER_END:; // makes clang happy
		}
		return nullptr;
	}

	std::string Token::getName() const
	{
		return getTypeName(type);
	}

	DataType Token::getReturnType() const
	{
		return DATA_EMPTY;
	}

	const SourceLocation& Token::getLeftmostSourceLocation() const
	{
		return loc;
	}

	[[noreturn]] void Token::throwUnexpected() const
	{
		getLeftmostSourceLocation().throwHere<ParseError>(std::move(std::string("Unexpected ").append(getName())));
	}

	void Token::expectType(TokenType expected_type) const
	{
		if (this->type != expected_type)
		{
			getLeftmostSourceLocation().throwHere<ParseError>(std::move(std::string("Expected ").append(getTypeName(expected_type)).append(", found ").append(getName())));
		}
	}
}
