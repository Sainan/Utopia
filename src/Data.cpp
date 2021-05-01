#include "Data.hpp"

#include "SourceLocation.hpp"
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

		case DATA_FUNC:
			return "function";

		case DATA_BOOL:
			return "boolean";
		}
		return nullptr;
	}

	std::string Data::getTypeErrorMessage(const DataType expected_type) const
	{
		return std::string("Expected ").append(getTypeName(expected_type)).append(", found ").append(getTypeName(this->type));
	}

	void Data::expectType(const DataType expected_type) const
	{
		if (this->type != expected_type)
		{
			throw TypeError(getTypeErrorMessage(expected_type));
		}
	}

	void Data::expectType(const DataType expected_type, const SourceLocation& loc) const
	{
		if (this->type != expected_type)
		{
			loc.throwHere<TypeError>(getTypeErrorMessage(expected_type));
		}
	}

	bool Data::equals(const Data& b) const
	{
		return type == b.type;
	}
}
