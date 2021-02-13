#pragma once

#include "Token.hpp"

namespace Utopia
{
	class TokenAssignment : public Token
	{
	public:
		explicit TokenAssignment(const SourceLocation& loc);

		[[nodiscard]] std::string getName() const final;
	};
}
