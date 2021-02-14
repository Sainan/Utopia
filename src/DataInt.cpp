#include "DataInt.hpp"

namespace Utopia
{
	DataInt::DataInt(long long value)
		: Data(DATA_INT), value(value)
	{
	}

	std::string DataInt::toString() const
	{
		return std::to_string(value);
	}

	std::string DataInt::toCPP() const
	{
		return std::string("std::make_unique<DataInt>(").append(toString()).append(1, ')');
	}

	std::unique_ptr<Data> DataInt::copy() const
	{
		return std::make_unique<DataInt>(value);
	}
}
