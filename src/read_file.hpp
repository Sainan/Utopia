#pragma once

#include <string>

namespace Utopia
{
	[[nodiscard]] std::string read_file(std::string& path);
	[[nodiscard]] std::string read_file(const char* path);
}
