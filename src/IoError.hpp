#pragma once

#include "Error.hpp"

namespace Utopia
{
	class IoError : public Error
	{
	public:
		using Error::Error;
	};
}
