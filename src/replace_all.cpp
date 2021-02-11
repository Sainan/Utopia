#include "replace_all.hpp"

namespace Utopia
{
	void replace_all(std::string& str, const std::string& from, const std::string& to)
	{
		// https://stackoverflow.com/a/24315631/4796321
		size_t start_pos = 0;
		while ((start_pos = str.find(from, start_pos)) != std::string::npos)
		{
			str.replace(start_pos, from.length(), to);
			start_pos += to.length();
		}
	}
}
