#pragma once

#include "Token.hpp"

namespace Utopia
{
	class TokenFunction : public Token
	{
	public:
		SourceLocation contents_start_loc;
		std::string contents;

		explicit TokenFunction(const SourceLocation& loc, const SourceLocation& contents_start_loc, std::string&& contents);

		[[nodiscard]] std::string getName() const final;

		[[nodiscard]] DataType getReturnType() const final;
	};
}
