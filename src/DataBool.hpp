#pragma once

#include "Data.hpp"

namespace Utopia
{
	class DataBool : public Data
	{
	public:
		bool value;

		explicit DataBool(bool value = false);

		[[nodiscard]] std::string toString() const final;
		[[nodiscard]] std::string toCPP() const final;

		[[nodiscard]] std::unique_ptr<Data> copy() const final;

		[[nodiscard]] bool equals(const Data& b) const final;
	};
}
