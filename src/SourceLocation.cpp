#include "SourceLocation.hpp"

#include "ParseError.hpp"

namespace Utopia
{
	SourceLocation::SourceLocation(std::string&& name)
		: name(std::move(name))
	{
	}

	void SourceLocation::throwHere(std::string&& error) const
	{
		throw ParseError(error.append(" in ").append(name).append(" on line ").append(std::to_string(line)).append(", colon ").append(std::to_string(colon)));
	}
}
