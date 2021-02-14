#pragma once

#include "VmError.hpp"

namespace Utopia
{
	class TypeError : public VmError
	{
	public:
		using VmError::VmError;
	};
}
