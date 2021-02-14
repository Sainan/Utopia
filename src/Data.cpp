#include "Data.hpp"

#include "TypeError.hpp"

namespace Utopia
{
	Data::Data(const DataType type)
		: type(type)
	{
	}

	Data::~Data()
	{
	}

	const char* Data::getTypeName(const DataType type)
	{
		switch (type)
		{
		case DATA_EMPTY:
			return "empty";

		case DATA_STRING:
			return "string";
			
		case DATA_INT:
			return "integer";
		}
		return nullptr;
	}

	void Data::expectType(const DataType expected_type) const
	{
		if (this->type != expected_type)
		{
			throw TypeError(std::string("Expected ").append(getTypeName(expected_type)).append(", found ").append(getTypeName(this->type)));
		}
	}
}
