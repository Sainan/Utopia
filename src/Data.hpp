#pragma once

#include <string>

namespace Utopia
{
	class Data
	{
	public:
		virtual ~Data();

		[[nodiscard]] virtual std::string toString() const = 0;

		[[nodiscard]] virtual std::string toCPP() const = 0;
	};
}
