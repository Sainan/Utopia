#include "Scope.hpp"

#include "VmError.hpp"
#include "TypeError.hpp"

#include "opcodes.hpp"
#include "Program.hpp"

namespace Utopia
{
	template <class E>
	__declspec(noreturn) static void vm_catch(const Scope* scope, Program* p, std::vector<uint8_t>::const_iterator& i, const E& e)
	{
		if (scope->op_locs.empty())
		{
			throw e;
		}
		static_cast<VmError>(e).rethrow(scope->op_locs.at(i - 1 - scope->ops.cbegin()));
	}

	void Scope::execute(Program* p) const
	{
#if DEBUG_VM
		if (p == this)
		{
			p->printVariables();
		}
#endif
		auto i = ops.cbegin();
		while (i != ops.cend())
		{
			try
			{
				auto opcode = *i++;
#if DEBUG_VM
				if (opcode >= _OP_SIZE)
				{
					throw VmError(std::string("Unknown opcode ").append(std::to_string(opcode)));
				}
				std::cout << std::to_string(opcode) << "(";
				for (auto num_ops = opcodes[opcode].num_args;; )
				{
					std::cout << std::to_string(*i++);
					if (--num_ops == 0)
					{
						break;
					}
					std::cout << ", ";
				}
				std::cout << ")" << std::endl;
				i -= opcodes[opcode].num_args;
#endif
				opcodes[opcode].execute(p, i);
			}
			catch (const TypeError& e)
			{
				vm_catch<>(this, p, i, e);
			}
			catch (const VmError& e)
			{
				vm_catch<>(this, p, i, e);
			}
		}
	}
}
