#include "SourceLocation.hpp"

namespace Utopia
{
	SourceLocation::SourceLocation(std::string&& name)
		: name(std::move(name))
	{
	}

	void SourceLocation::newline()
	{
		line++;
		character = 1;
	}

	std::string SourceLocation::toString() const
	{
		return std::string(name).append(" on line ").append(std::to_string(line)).append(", character ").append(std::to_string(character));
	}

	std::string SourceLocation::getSuffix() const
	{
		return std::string(" in ").append(toString());
	}
}
