#include "DataString.hpp"

#include "escape_string.hpp"

namespace Utopia
{
	DataString::DataString(std::string&& value)
		: value(std::move(value))
	{
	}

	std::string DataString::toString() const
	{
		return std::string(value);
	}

	std::string DataString::toCPP() const
	{
		return std::string("std::make_unique<DataString>(\"").append(escape_string(value)).append("\")");
	}
}
