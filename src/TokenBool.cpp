#include "TokenBool.hpp"

namespace Utopia
{
	TokenBool::TokenBool(const SourceLocation& loc, bool value)
		: Token(TOKEN_BOOL, loc), value(value)
	{
	}

	std::string TokenBool::getName() const
	{
		return std::string("boolean ").append(value ? "true" : "false");
	}

	DataType TokenBool::getReturnType() const
	{
		return DATA_BOOL;
	}
}
