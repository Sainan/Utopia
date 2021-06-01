#pragma once

#include "TokenContainer.hpp"

namespace Utopia
{
	class TokenComparator : public TokenContainer
	{
	public:
		using TokenContainer::TokenContainer;

		[[nodiscard]] DataType getReturnType() const final;
	};
}
