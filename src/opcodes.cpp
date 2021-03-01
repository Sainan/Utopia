#include "opcodes.hpp"

#include "DataFunction.hpp"
#include "DataInt.hpp"
#include "DataString.hpp"

#include "arithmetic.hpp"
#include "int_from_byte_stream.hpp"
#include "Program.hpp"
#include "VmError.hpp"

namespace Utopia
{
	static std::unique_ptr<Data>* getVariable(Program* p, std::vector<uint8_t>::const_iterator& i)
	{
		return &p->variables.at(int_from_byte_stream(i));
	}

	static std::unique_ptr<Data>* peekVariable(Program* p, std::vector<uint8_t>::const_iterator& i, uint8_t look_ahead = 0)
	{
		i += (look_ahead * 4);
		auto res = getVariable(p, i);
		i -= ((look_ahead + 1) * 4);
		return res;
	}

	template <typename T>
	[[nodiscard]] static T* getT(Program* p, std::vector<uint8_t>::const_iterator& i, const DataType type)
	{
		std::unique_ptr<Data>* elm = getVariable(p, i);
		if ((*elm)->type == DATA_EMPTY)
		{
			*elm = std::make_unique<T>();
		}
		else
		{
			(*elm)->expectType(type);
		}
		return (T*)elm->get();
	}

	[[nodiscard]] static long long* getInt(Program* p, std::vector<uint8_t>::const_iterator& i)
	{
		return &getT<DataInt>(p, i, DATA_INT)->value;
	}

	[[nodiscard]] static std::string* getString(Program* p, std::vector<uint8_t>::const_iterator& i)
	{
		return &getT<DataString>(p, i, DATA_STRING)->value;
	}

	[[nodiscard]] static std::string getStringValue(Program* p, std::vector<uint8_t>::const_iterator& i)
	{
		Data* var = (*getVariable(p, i)).get();
		if (var->type == DATA_INT)
		{
			return std::to_string(((DataInt*)var)->value);
		}
		var->expectType(DATA_STRING);
		return ((const DataString*)var)->value;
	}

	static void arithmetic_op(Program* p, std::vector<uint8_t>::const_iterator& i, arithmetic_func_t arithmetic_func)
	{
		auto* out = getInt(p, i);
		*out = arithmetic_func(*getInt(p, i), *getInt(p, i));
	}

	static void concat(Program* p, std::vector<uint8_t>::const_iterator& i)
	{
		auto* out = getString(p, i);
		*out = std::move(std::string(getStringValue(p, i)).append(getStringValue(p, i)));
	}

	OpCodeData opcodes[] = {
		// OP_ECHO
		{
			1,
			[](Program* p, std::vector<uint8_t>::const_iterator& i)
			{
				auto str = (*getVariable(p, i))->toString();
				p->echo_func(str.c_str(), p->echo_func_arg);
			}
		},
		// OP_PLUS
		{
			3,
			[](Program* p, std::vector<uint8_t>::const_iterator& i)
			{
				if ((*peekVariable(p, i, 1))->type == DATA_STRING || (*peekVariable(p, i, 2))->type == DATA_STRING)
				{
					concat(p, i);
				}
				else
				{
					arithmetic_op(p, i, &arithmetic_plus);
				}
			}
		},
		// OP_MINUS
		{
			3,
			[](Program* p, std::vector<uint8_t>::const_iterator& i)
			{
				arithmetic_op(p, i, &arithmetic_minus);
			}
		},
		// OP_MULTIPLY
		{
			3,
			[](Program* p, std::vector<uint8_t>::const_iterator& i)
			{
				arithmetic_op(p, i, &arithmetic_multiply);
			}
		},
		// OP_DIVIDE
		{
			3,
			[](Program* p, std::vector<uint8_t>::const_iterator& i)
			{
				arithmetic_op(p, i, &arithmetic_divide);
			}
		},
		// OP_ASSIGNMENT
		{
			2,
			[](Program* p, std::vector<uint8_t>::const_iterator& i)
			{
				std::unique_ptr<Data>* left = getVariable(p, i);
				*left = (*getVariable(p, i))->copy();
			}
		},
		// OP_CONCAT
		{
			3,
			[](Program* p, std::vector<uint8_t>::const_iterator& i)
			{
				concat(p, i);
			}
		},
		// OP_CALL
		{
			1,
			[](Program* p, std::vector<uint8_t>::const_iterator& i)
			{
				Data* func = (*getVariable(p, i)).get();
				func->expectType(DATA_FUNC);
				((DataFunction*)func)->scope.execute(p);
			}
		}
	};
}
