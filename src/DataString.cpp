#include "DataString.hpp"

#include "replace_all.hpp"

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
		std::string value(this->value);
		replace_all(value, "\\", "\\\\");
		replace_all(value, "\"", "\\\"");
		return std::string("std::make_unique<DataString>(\"").append(value).append("\")");
	}
}
