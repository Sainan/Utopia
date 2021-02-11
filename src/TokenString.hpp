#pragma once

#include "Token.hpp"

#include <string>

namespace Utopia
{
	class TokenString : public Token
	{
	public:
		std::string value;

		explicit TokenString(std::string&& value);
	};
}
