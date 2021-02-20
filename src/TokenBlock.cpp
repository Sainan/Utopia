#include "TokenBlock.hpp"

#include "escape_block_string.hpp"

namespace Utopia
{
	TokenBlock::TokenBlock(const SourceLocation& loc)
		: Token(TOKEN_BLOCK, loc)
	{
	}

	std::string TokenBlock::getName() const
	{
		return std::string("block \"").append(escape_block_string(contents)).append(1, '"');
	}
}
