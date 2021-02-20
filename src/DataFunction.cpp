#include "DataFunction.hpp"

namespace Utopia
{
	DataFunction::DataFunction(Scope&& scope)
		: Data(DATA_FUNC), scope(std::move(scope))
	{
	}

	std::string DataFunction::toString() const
	{
		return "function";
	}

	std::string DataFunction::toCPP() const
	{
		std::string str("std::make_unique<DataFunction>(Scope{{");
		for (const auto& op : scope.ops)
		{
			str.append(std::to_string(op)).append(",");
		}
		str.append("}})");
		return str;
	}

	std::unique_ptr<Data> DataFunction::copy() const
	{
		return std::make_unique<DataFunction>(Scope{ scope });
	}
}
