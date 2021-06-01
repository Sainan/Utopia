#pragma once

#include "TokenComparator.hpp"

namespace Utopia
{
	class TokenUnequal : public TokenComparator
	{
	public:
		explicit TokenUnequal(const SourceLocation& loc);
	};
}
