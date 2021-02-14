#pragma once

#include "fwddecl.hpp"

#include "Token.hpp"

#include <memory>

namespace Utopia
{
	class TokenContainer : public Token
	{
	public:
		std::unique_ptr<Token> left{};
		std::unique_ptr<Token> right{};

	protected:
		using Token::Token;

	public:
		[[nodiscard]] const SourceLocation& getLeftmostSourceLocation() const final;

		[[nodiscard]] virtual std::unique_ptr<Data> attemptToEvaluate() const;
	};
}
