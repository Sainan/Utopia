#include "TokenContainer.hpp"

namespace Utopia
{
	const SourceLocation& TokenContainer::getLeftmostSourceLocation() const
	{
		if (left)
		{
			return left->getLeftmostSourceLocation();
		}
		return Token::getLeftmostSourceLocation();
	}
}
