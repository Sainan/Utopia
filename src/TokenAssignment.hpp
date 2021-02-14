#pragma once

#include "TokenContainer.hpp"

namespace Utopia
{
	class TokenAssignment : public TokenContainer
	{
	public:
		explicit TokenAssignment(const SourceLocation& loc);
	};
}
