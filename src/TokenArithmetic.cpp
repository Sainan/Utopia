#include "TokenArithmetic.hpp"

#include "DataInt.hpp"
#include "TokenInt.hpp"

namespace Utopia
{
	TokenArithmetic::TokenArithmetic(const TokenType type, const SourceLocation& loc, arithmetic_func_t arithmetic_func)
		: TokenContainer(type, loc), arithmetic_func(arithmetic_func)
	{
	}

	bool TokenArithmetic::isRValue() const
	{
		return left->isRValue() && right->isRValue();
	}

	[[nodiscard]] static long long coerceInt(Token* token)
	{
		if (token->isContainer())
		{
			if (std::unique_ptr<Data> data = ((TokenContainer*)token)->attemptToEvaluate())
			{
				data->expectType(DATA_INT);
				return ((DataInt*)data.get())->value;
			}
		}
		token->expectType(TOKEN_INT);
		return ((TokenInt*)token)->value;
	}

	std::unique_ptr<Data> TokenArithmetic::attemptToEvaluate() const
	{
		if (isRValue())
		{
			return std::make_unique<DataInt>(arithmetic_func(coerceInt(left.get()), coerceInt(right.get())));
		}
		return TokenContainer::attemptToEvaluate();
	}
}
