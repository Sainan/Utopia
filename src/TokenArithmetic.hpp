#pragma once

#include "TokenContainer.hpp"

#include "arithmetic.hpp"

namespace Utopia
{
	class TokenArithmetic : public TokenContainer
	{
	private:
		const arithmetic_func_t arithmetic_func;

	protected:
		explicit TokenArithmetic(const TokenType type, const SourceLocation& loc, arithmetic_func_t arithmetic_func);

	public:
		[[nodiscard]] bool isRValue() const final;

		[[nodiscard]] std::unique_ptr<Data> attemptToEvaluate() const final;
	};
}
