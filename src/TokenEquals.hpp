#pragma once

#include "TokenContainer.hpp"

namespace Utopia
{
	class TokenEquals : public TokenContainer
	{
	public:
		explicit TokenEquals(const SourceLocation& loc);
	};
}
