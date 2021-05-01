#include "DataString.hpp"

#include "escape_string.hpp"

namespace Utopia
{
	DataString::DataString(std::string&& value)
		: Data(DATA_STRING), value(std::move(value))
	{
	}

	DataString::DataString(const std::string& value)
		: Data(DATA_STRING), value(value)
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

	std::unique_ptr<Data> DataString::copy() const
	{
		return std::make_unique<DataString>(value);
	}

	bool DataString::equals(const Data& b) const
	{
		return Data::equals(b) && reinterpret_cast<const DataString&>(b).value == value;
	}
}
