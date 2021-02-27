#include "config.hpp"
#if defined(__clang__) || UTO_MODE == UTO_INTERPRETER

#include <iostream>

#include "Error.hpp"
#include "Program.hpp"

using namespace Utopia;

int main(int num_args, const char** args)
{
	const char* path = nullptr;
	bool lint = false;
	if (num_args < 2)
	{
	invalid_syntax:
		std::cout << "Syntax: " << args[0] << " [--lint] <uto-file>" << std::endl;
		return 1;
	}
	for (int i = 1; i != num_args; i++)
	{
		if (strcmp(args[i], "--lint") == 0 || strcmp(args[i], "-l") == 0)
		{
			lint = true;
			continue;
		}
		if (path != nullptr)
		{
			goto invalid_syntax;
		}
		path = args[i];
	}

	try
	{
		Program p{};
		bool any_warnings = false;
		p.warn_func = [](const Warning* warning, void* arg)
		{
			Program::warn_impl_stderr(warning, arg);
			*(bool*)arg = true;
		};
		p.warn_func_arg = &any_warnings;
		p.fromFile(path);
		if (lint)
		{
			if (!any_warnings)
			{
				std::cout << "No syntax errors detected in " << path << std::endl;
			}
		}
		else
		{
			p.execute();
		}
	}
	catch (const Error& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}

#endif
