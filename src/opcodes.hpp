#pragma once

#include "fwddecl.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace Utopia
{
	enum OpCode : uint8_t
	{
		OP_ECHO = 0,
		OP_PLUS,
		OP_MINUS,
		OP_MULTIPLY,
		OP_DIVIDE,
		OP_ASSIGNMENT,
		OP_CONCAT,

		_OP_SIZE
	};

	using op_execute_t = void(*)(Program&, std::vector<uint8_t>::iterator&);

	struct OpCodeData
	{
		op_execute_t execute;
	};

	extern OpCodeData opcodes[_OP_SIZE];
}
