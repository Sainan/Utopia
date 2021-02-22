#pragma once

#include "Error.hpp"

namespace Utopia
{
	// pointless class, but at least no circular include.
	class TracableError : public Error
	{
	protected:
		bool got_loc = false;

	public:
		explicit TracableError(const std::string& reason, bool got_loc = false);
	};
}
