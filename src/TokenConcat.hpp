#pragma once

#include "TokenContainer.hpp"

namespace Utopia
{
	class TokenConcat : public TokenContainer
	{
	public:
		explicit TokenConcat(const SourceLocation& loc);

		[[nodiscard]] bool isRValue() const final;

		[[nodiscard]] std::unique_ptr<Data> attemptToEvaluate() const final;
	};
}
