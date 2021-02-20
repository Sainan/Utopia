#include "opcodes.hpp"

#include "DataFunction.hpp"
#include "DataInt.hpp"
#include "DataString.hpp"

#include "arithmetic.hpp"
#include "Program.hpp"
#include "VmError.hpp"

namespace Utopia
{
	template <typename T>
	[[nodiscard]] static T* getT(Program& p, std::vector<uint8_t>::const_iterator& i, const DataType type)
	{
		std::unique_ptr<Data>* elm = &p.variables.at(*i++);
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

	[[nodiscard]] static long long* getInt(Program& p, std::vector<uint8_t>::const_iterator& i)
	{
		return &getT<DataInt>(p, i, DATA_INT)->value;
	}

	[[nodiscard]] static std::string* getString(Program& p, std::vector<uint8_t>::const_iterator& i)
	{
		return &getT<DataString>(p, i, DATA_STRING)->value;
	}

	[[nodiscard]] static std::string getStringValue(Program& p, std::vector<uint8_t>::const_iterator& i)
	{
		Data* var = p.variables.at(*i++).get();
		if (var->type == DATA_INT)
		{
			return std::to_string(((DataInt*)var)->value);
		}
		var->expectType(DATA_STRING);
		return ((const DataString*)var)->value;
	}

	static void arithmetic_op(Program& p, std::vector<uint8_t>::const_iterator& i, arithmetic_func_t arithmetic_func)
	{
		auto* out = getInt(p, i);
		*out = arithmetic_func(*getInt(p, i), *getInt(p, i));
	}

	static void concat(Program& p, std::vector<uint8_t>::const_iterator& i)
	{
		auto* out = getString(p, i);
		*out = std::move(std::string(getStringValue(p, i)).append(getStringValue(p, i)));
	}

	OpCodeData opcodes[] = {
		// OP_ECHO
		{
			[](Program& p, std::vector<uint8_t>::const_iterator& i)
			{
				auto str = p.variables.at(*i++)->toString();
				p.echo_func(str.c_str());
			}
		},
		// OP_PLUS
		{
			[](Program& p, std::vector<uint8_t>::const_iterator& i)
			{
				if (p.variables.at(*(i + 1))->type == DATA_STRING || p.variables.at(*(i + 2))->type == DATA_STRING)
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
			[](Program& p, std::vector<uint8_t>::const_iterator& i)
			{
				arithmetic_op(p, i, &arithmetic_minus);
			}
		},
		// OP_MULTIPLY
		{
			[](Program& p, std::vector<uint8_t>::const_iterator& i)
			{
				arithmetic_op(p, i, &arithmetic_multiply);
			}
		},
		// OP_DIVIDE
		{
			[](Program& p, std::vector<uint8_t>::const_iterator& i)
			{
				arithmetic_op(p, i, &arithmetic_divide);
			}
		},
		// OP_ASSIGNMENT
		{
			[](Program& p, std::vector<uint8_t>::const_iterator& i)
			{
				std::unique_ptr<Data>* left = &p.variables.at(*i++);
				*left = p.variables.at(*i++)->copy();
			}
		},
		// OP_CONCAT
		{
			[](Program& p, std::vector<uint8_t>::const_iterator& i)
			{
				concat(p, i);
			}
		},
		// OP_CALL
		{
			[](Program& p, std::vector<uint8_t>::const_iterator& i)
			{
				Data* func = p.variables.at(*i++).get();
				func->expectType(DATA_FUNC);
				const Scope& scope = ((DataFunction*)func)->scope;
				auto j = scope.ops.cbegin();
				while (j != scope.ops.cend())
				{
					opcodes[*j++].execute(p, j);
				}
			}
		}
	};
}
