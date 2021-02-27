#pragma once

#include <string>

#include "SourceLocation.hpp"

namespace Utopia
{
	struct Warning
	{
		const std::string message;
		const SourceLocation loc;

		explicit Warning(std::string&& message, const SourceLocation& loc);

		[[nodiscard]] std::string toString() const;
	};
}
