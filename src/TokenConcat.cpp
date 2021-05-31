#include "TokenConcat.hpp"

namespace Utopia
{
	TokenConcat::TokenConcat(const SourceLocation& loc)
		: TokenContainer(TOKEN_CONCAT, loc)
	{
	}

	TokenConcat::TokenConcat(TokenContainer&& b)
		: TokenContainer(TOKEN_CONCAT, std::move(b))
	{
	}

	DataType TokenConcat::getReturnType() const
	{
		return DATA_STRING;
	}
}
