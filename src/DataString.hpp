#pragma once

#include "Data.hpp"

namespace Utopia
{
	class DataString : public Data
	{
	public:
		std::string value;

		DataString(std::string&& value);

		[[nodiscard]] std::string toString() const final;

		[[nodiscard]] std::string toCPP() const final;
	};
}
