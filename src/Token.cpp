#include "Token.hpp"

#include "ParseError.hpp"

namespace Utopia
{
	Token::Token(TokenType type, size_t line_num)
		: type(type), line_num(line_num)
	{
	}

	void Token::throwUnexpected() const
	{
		throw ParseError(std::string("Unexpected ").append(getName()).append(" on line ").append(std::to_string(line_num)));
	}

	Token::~Token()
	{
	}

	void Token::expectType(TokenType expected_type) const
	{
		if (this->type != expected_type)
		{
			throwUnexpected();
		}
	}
}
