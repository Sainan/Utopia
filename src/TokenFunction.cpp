#include "TokenFunction.hpp"

#include "escape_block_string.hpp"

namespace Utopia
{
	TokenFunction::TokenFunction(const SourceLocation& loc, std::string&& contents)
		: Token(TOKEN_FUNC, loc), contents(std::move(contents))
	{
	}

	std::string TokenFunction::getName() const
	{
		return std::string("function \"").append(escape_block_string(contents)).append(1, '"');
	}
}
