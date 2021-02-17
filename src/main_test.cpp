#include "config.hpp"
#if defined(__clang__) || UTO_MODE == UTO_TEST

#include <functional>
#include <iostream>

#include "Program.hpp"

#include "ParseError.hpp"
#include "VmError.hpp"

using namespace Utopia;

static int total_tests;
static int successful_tests;
static std::string out_buf;

static void initProgramTest(const char* code, std::function<bool(Program&)>&& callback)
{
	total_tests++;
	out_buf = {};
	Program p{};
	try
	{
		p.fromString("test", code);
	}
	catch (const ParseError& e)
	{
		std::cout << e.what() << std::endl;
		return;
	}
	p.echo_func = [](const char* str)
	{
		out_buf.append(str);
	};
	if (callback(p))
	{
		successful_tests++;
	}
}

static void assertOutput(const char* name, const char* code, const char* expected_output)
{
	std::cout << name << ": ";
	initProgramTest(code, [=](Program& p)
	{
		try
		{
			p.execute();
		}
		catch (const VmError& e)
		{
			std::cout << e.what() << std::endl;
			return false;
		}
		if (out_buf != expected_output)
		{
			std::cout << "expected '" << expected_output << "' as output but got '" << out_buf << "'" << std::endl;
			return false;
		}
		std::cout << "OK" << std::endl;
		return true;
	});
}

int main()
{
	total_tests = 0;
	successful_tests = 0;
	
	assertOutput("Order of Operations", R"(echo 65 + 2 * 2)", "69");
	assertOutput("Named Variable", R"(hello = "Hello, world!"; echo hello)", "Hello, world!");
	assertOutput("By-Value", R"(two = 2; res = 65 + two * 2; echo res + " " + two)", "69 2");
	assertOutput("Variable Override", R"(res = 40; res = res + 2; echo res)", "42");

	std::cout << "Compile-time concat: ";
	initProgramTest(R"(echo "A" "B")", [](Program& p)
	{
		if (p.variables.size() != 1)
		{
			std::cout << "expected 1 variable but program has " << p.variables.size() << std::endl;
			return false;
		}
		std::cout << "OK" << std::endl;
		return true;
	});

	std::cout << successful_tests << " / " << total_tests << " tests succeeded." << std::endl;

	return total_tests == successful_tests ? 0 : 1;
}

#endif