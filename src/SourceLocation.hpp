#pragma once

#include <string>

#include "Shared.hpp"

namespace Utopia
{
	struct SourceLocation
	{
		Shared<std::string> name;
		size_t line = 0;
		size_t colon = 0;
		
		explicit SourceLocation(std::string&& name);

		[[nodiscard]] std::string toString() const;
	};
}
