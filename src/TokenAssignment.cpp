#include "TokenAssignment.hpp"

namespace Utopia
{
	TokenAssignment::TokenAssignment(const SourceLocation& loc)
		: Token(TOKEN_ASSIGNMENT, loc)
	{
	}

	std::string TokenAssignment::getName() const
	{
		return "assignment";
	}
}
