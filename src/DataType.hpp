#pragma once

#include <cstdint>

namespace Utopia
{
	enum DataType : int8_t
	{
		DATA_EMPTY = -1,
		DATA_STRING = 0,
		DATA_INT,
		DATA_FUNC,
		DATA_BOOL,
	};
}
