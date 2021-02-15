#include "read_file.hpp"

#include <fstream>

#include "IoError.hpp"

namespace Utopia
{
	std::string read_file(std::string& path)
	{
		std::ifstream file(path);
		file.seekg(0, std::ios::end);
		auto size = (size_t)file.tellg();
		if (size == std::string::npos)
		{
			throw IoError(path.insert(0, "Failed to open "));
		}
		std::string buffer(size, ' ');
		file.seekg(0);
		file.read(&buffer[0], size);
		return buffer;
	}

	std::string read_file(const char* path)
	{
		std::string path_str(path);
		return read_file(path_str);
	}
}
