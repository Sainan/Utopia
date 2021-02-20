#pragma once

#include <string>

namespace Utopia
{
	[[nodiscard]] extern std::string escape_string(const std::string& in);
	extern void escape_string(std::string& str);
}
