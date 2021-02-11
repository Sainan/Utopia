#include "Token.hpp"

#include "ParseError.hpp"

namespace Utopia
{
	Token::Token(TokenType type, const SourceLocation& loc)
		: type(type), loc(loc)
	{
	}

	void Token::throwUnexpected() const
	{
		throw ParseError(std::string("Unexpected ").append(getName()).append(" in ").append(loc.toString()));
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
