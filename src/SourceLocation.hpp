#pragma once

#include <string>

#include "Shared.hpp"

namespace Utopia
{
	struct SourceLocation
	{
		Shared<std::string> name;
		size_t line = 1;
		size_t colon = 0;
		
		explicit SourceLocation(std::string&& name);

		void throwHere(std::string&& error) const;
	};
}
