#pragma once

#include "TokenComparator.hpp"

namespace Utopia
{
	class TokenEquals : public TokenComparator
	{
	public:
		explicit TokenEquals(const SourceLocation& loc);
	};
}
