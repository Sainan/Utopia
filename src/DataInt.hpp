#pragma once

#include "Data.hpp"

namespace Utopia
{
	class DataInt : public Data
	{
	public:
		long long value;

		explicit DataInt(long long value = 0);

		[[nodiscard]] std::string toString() const final;
		[[nodiscard]] std::string toCPP() const final;

		[[nodiscard]] std::unique_ptr<Data> copy() const final;
	};
}
