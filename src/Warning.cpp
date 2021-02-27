#include "Warning.hpp"

namespace Utopia
{
	Warning::Warning(std::string&& message, const SourceLocation& loc)
		: message(std::move(message)), loc(loc)
	{
	}

	std::string Warning::toString() const
	{
		return std::string(message).append(loc.getSuffix());
	}
}
