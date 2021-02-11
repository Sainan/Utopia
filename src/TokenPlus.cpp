#include "TokenPlus.hpp"

namespace Utopia
{
	TokenPlus::TokenPlus(size_t line_num)
		: Token(TOKEN_PLUS, line_num)
	{
	}

	std::string TokenPlus::getName() const
	{
		return "plus";
	}
}
