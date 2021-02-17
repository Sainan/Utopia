#include "config.hpp"
#if defined(__clang__) || UTO_MODE == UTO_BENCHMARK

#include <iostream>

#include "read_file.hpp"
#include "Profiling.hpp"
#include "Program.hpp"

using namespace Utopia;

int main()
{
#if PERF_PROFILING
	std::cout << "Don't compile the benchmark with PERF_PROFILING enabled." << std::endl;
#else
	const char* files[] = {
		"examples/hello.uto",
		"examples/calc.uto",
	};
	Program p{};
	for (const auto& file : files)
	{
		std::string code = read_file(file);
		Profiling::startSection(file);
		for (int i = 0; i < 100000; i++)
		{
			p.fromString("", code);
		}
		Profiling::endSection(file);
	}
#endif
}

#endif
