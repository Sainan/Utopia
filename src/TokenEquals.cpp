#include "TokenEquals.hpp"

namespace Utopia
{
	TokenEquals::TokenEquals(const SourceLocation& loc)
		: TokenContainer(TOKEN_EQUALS, loc)
	{
	}

	DataType TokenEquals::getReturnType() const
	{
		return DATA_BOOL;
	}
}
