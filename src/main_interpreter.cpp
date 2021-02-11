#include <iostream>

#include "Program.hpp"

using namespace Utopia;

int main(int num_args, const char** args)
{
	if (num_args != 2)
	{
		std::cout << "Syntax: " << args[0] << " <ul-file>" << std::endl;
		return 1;
	}

	Program p = Program::fromFile(args[1]);
	p.execute();
	return 0;
}
