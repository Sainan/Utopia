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
		
		for (auto i = scope.ops.cbegin(); i != scope.ops.cend(); i++)
		{
			if (i != scope.ops.cbegin())
			{
				str.append(1, ',');
			}
			str.append(std::to_string(*i));
		}
		str.append("}})");
		return str;
	}

	std::unique_ptr<Data> DataFunction::copy() const
	{
		return std::make_unique<DataFunction>(Scope{ scope });
	}
}
