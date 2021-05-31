#include "TokenFunction.hpp"

#include "escape_block_string.hpp"

namespace Utopia
{
	TokenFunction::TokenFunction(const SourceLocation& loc, const SourceLocation& contents_start_loc, std::string&& contents)
		: Token(TOKEN_FUNC, loc), contents_start_loc(contents_start_loc), contents(std::move(contents))
	{
	}

	std::string TokenFunction::getName() const
	{
		return std::string("function \"").append(escape_block_string(contents)).append(1, '"');
	}

	DataType TokenFunction::getReturnType() const
	{
		return DATA_FUNC;
	}
}
