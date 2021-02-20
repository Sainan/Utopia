#include "config.hpp"
#if defined(__clang__) || UTO_MODE == UTO_COMPILER

#include <fstream>
#include <iostream>

#include "ParseError.hpp"
#include "Program.hpp"

using namespace Utopia;

int main(int num_args, const char** args)
{
	if (num_args != 2)
	{
		std::cout << "Syntax: " << args[0] << " <uto-file>" << std::endl;
		return 1;
	}

	try
	{
		Program p{};
		p.fromFile(args[1]);

		std::ofstream file("src/main_compiled.cpp");
		file << R"EOC(#include "DataEmpty.hpp"
#include "DataFunction.hpp"
#include "DataInt.hpp"
#include "DataString.hpp"
#include "Program.hpp"

using namespace Utopia;

int main()
{
	Program p{};
)EOC";

		for (const auto& variable : p.variables)
		{
			file << "p.variables.emplace_back(" << variable->toCPP() << ");";
		}

		file << "p.ops = {";
		for (size_t i = 0; i < p.ops.size(); i++)
		{
			if (i != 0)
			{
				file << ",";
			}
			file << std::to_string(p.ops.at(i));
		}
		file << "};";

		file << "p.execute();}";
	}
	catch (const ParseError& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}

#endif
