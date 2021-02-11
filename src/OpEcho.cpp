#include "OpEcho.hpp"

#include <iostream>

#include "Program.hpp"

namespace Utopia
{
	size_t OpEcho::getNumArgs() const
	{
		return 1;
	}

	void OpEcho::execute(const Program& p, std::vector<uint8_t>&& args) const
	{
		std::cout << p.data.at(args.at(0))->toString();
	}
}
