#include "SourceLocation.hpp"

namespace Utopia
{
	SourceLocation::SourceLocation(std::string&& name)
		: name(std::move(name))
	{
	}

	std::string SourceLocation::getSuffix() const
	{
		return std::string(" in ").append(name).append(" on line ").append(std::to_string(line)).append(", character ").append(std::to_string(colon));
	}
}
