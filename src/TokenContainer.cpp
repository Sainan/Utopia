#include "TokenContainer.hpp"

#include "Data.hpp"

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

	std::unique_ptr<Data> TokenContainer::attemptToEvaluate() const
	{
		return {};
	}
}
