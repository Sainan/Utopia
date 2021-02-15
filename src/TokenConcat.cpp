#include "TokenConcat.hpp"

#include "DataString.hpp"
#include "TokenString.hpp"

namespace Utopia
{
	TokenConcat::TokenConcat(const SourceLocation& loc)
		: TokenContainer(TOKEN_CONCAT, loc)
	{
	}

	bool TokenConcat::isRValue() const
	{
		return left->isRValue() && right->isRValue();
	}

	[[nodiscard]] static std::string coerceString(Token* token)
	{
		if (token->isContainer())
		{
			if (std::unique_ptr<Data> data = ((TokenContainer*)token)->attemptToEvaluate())
			{
				data->expectType(DATA_STRING);
				return ((DataString*)data.get())->value;
			}
		}
		token->expectType(TOKEN_STRING);
		return ((TokenString*)token)->value;
	}

	std::unique_ptr<Data> TokenConcat::attemptToEvaluate() const
	{
		if (isRValue())
		{
			return std::make_unique<DataString>(coerceString(left.get()).append(coerceString(right.get())));
		}
		return TokenContainer::attemptToEvaluate();
	}
}
