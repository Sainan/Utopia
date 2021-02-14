#include "config.hpp"
#if defined(__clang__) || UTO_MODE == UTO_INTERPRETER

#include <iostream>

#include "Error.hpp"
#include "Program.hpp"

using namespace Utopia;

int main(int num_args, const char** args)
{
	if (num_args != 2)
	{
		std::cout << "Syntax: " << args[0] << " <ul-file>" << std::endl;
		return 1;
	}

	try
	{
		Program p = Program::fromFile(args[1]);
		p.execute();
	}
	catch (const Error& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}

#endif
