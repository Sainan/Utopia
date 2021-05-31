#include "TokenConcat.hpp"

namespace Utopia
{
	TokenConcat::TokenConcat(const SourceLocation& loc)
		: TokenContainer(TOKEN_CONCAT, loc)
	{
	}

	DataType TokenConcat::getReturnType() const
	{
		return DATA_STRING;
	}
}
