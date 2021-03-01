#include "int_from_byte_stream.hpp"

namespace Utopia
{
	uint32_t int_from_byte_stream(std::vector<uint8_t>::const_iterator& i)
	{
		uint32_t res = *i++;
		res <<= 8;
		res |= *i++;
		res <<= 8;
		res |= *i++;
		res <<= 8;
		res |= *i++;
		return res;
	}
}
