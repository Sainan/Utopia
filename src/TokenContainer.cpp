#include "TokenContainer.hpp"

namespace Utopia
{
	TokenContainer::TokenContainer(TokenType type, TokenContainer&& b)
		: Token(type, b.loc), left(std::move(b.left)), right(std::move(b.right))
	{
	}

	const SourceLocation& TokenContainer::getLeftmostSourceLocation() const
	{
		if (left)
		{
			return left->getLeftmostSourceLocation();
		}
		return Token::getLeftmostSourceLocation();
	}
}
