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

	void Token::throwUnexpected() const
	{
		loc.throwHere(std::move(std::string("Unexpected ").append(getName())));
	}

	void Token::expectType(TokenType expected_type) const
	{
		if (this->type != expected_type)
		{
			throwUnexpected();
		}
	}
}
