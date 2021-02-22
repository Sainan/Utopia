#include "TracableError.hpp"

namespace Utopia
{
	TracableError::TracableError(const std::string& reason, bool got_loc)
		: Error(reason), got_loc(got_loc)
	{
	}
}
