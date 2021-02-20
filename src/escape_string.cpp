#include "escape_string.hpp"

#include "replace_all.hpp"

namespace Utopia
{
	std::string escape_string(const std::string& in)
	{
		std::string out(in);
		escape_string(out);
		return out;
	}

	void escape_string(std::string& str)
	{
		replace_all(str, "\\", "\\\\");
		replace_all(str, "\"", "\\\"");
	}
}
