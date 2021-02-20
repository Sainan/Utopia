#include "escape_block_string.hpp"

#include "escape_string.hpp"
#include "replace_all.hpp"
#include "trim.hpp"

namespace Utopia
{
	std::string escape_block_string(const std::string& in)
	{
		std::string out(in);
		replace_all(out, "\r", "");
		replace_all(out, "\n", "");
		trim(out);
		escape_string(out);
		return out;
	}
}
