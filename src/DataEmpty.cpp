#include "DataEmpty.hpp"

namespace Utopia
{
	DataEmpty::DataEmpty()
		: Data(DATA_EMPTY)
	{
	}

	std::string DataEmpty::toString() const
	{
		return "";
	}

	std::string DataEmpty::toCPP() const
	{
		return "std::make_unique<DataEmpty>()";
	}

	std::unique_ptr<Data> DataEmpty::copy() const
	{
		return std::make_unique<DataEmpty>();
	}
}
