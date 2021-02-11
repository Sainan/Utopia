#pragma once

#include <stdexcept>

namespace Utopia
{
	class ParseError : public std::runtime_error
	{
	public:
		using std::runtime_error::runtime_error;
	};
}
