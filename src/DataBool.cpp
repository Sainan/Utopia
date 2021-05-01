#include "DataBool.hpp"

namespace Utopia
{
	DataBool::DataBool(bool value)
		: Data(DATA_BOOL), value(value)
	{
	}

	std::string DataBool::toString() const
	{
		return value ? "true" : "false";
	}

	std::string DataBool::toCPP() const
	{
		return std::string("std::make_unique<DataBool>(").append(toString()).append(1, ')');
	}

	std::unique_ptr<Data> DataBool::copy() const
	{
		return std::make_unique<DataBool>(value);
	}

	bool DataBool::equals(const Data& b) const
	{
		return Data::equals(b) && reinterpret_cast<const DataBool&>(b).value == value;
	}
}
