#include "escape_string.hpp"

#include "replace_all.hpp"

namespace Utopia
{
	std::string escape_string(const std::string& in)
	{
		std::string out(in);
		replace_all(out, "\\", "\\\\");
		replace_all(out, "\"", "\\\"");
		return std::move(out);
	}
}
