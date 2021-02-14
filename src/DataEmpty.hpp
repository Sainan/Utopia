#pragma once

#include "Data.hpp"

namespace Utopia
{
	class DataEmpty : public Data
	{
	public:
		explicit DataEmpty();

		[[nodiscard]] std::string toString() const final;
		[[nodiscard]] std::string toCPP() const final;

		[[nodiscard]] std::unique_ptr<Data> copy() const final;
	};
}
