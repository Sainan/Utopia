#pragma once

#include "Error.hpp"

namespace Utopia
{
	class VmError : public Error
	{
	public:
		using Error::Error;
	};
}
