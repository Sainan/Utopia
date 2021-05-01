#pragma once

#include "Data.hpp"

namespace Utopia
{
	class DataString : public Data
	{
	public:
		std::string value;

		explicit DataString(std::string&& value = {});
		explicit DataString(const std::string& value);

		[[nodiscard]] std::string toString() const final;
		[[nodiscard]] std::string toCPP() const final;

		[[nodiscard]] std::unique_ptr<Data> copy() const final;

		[[nodiscard]] bool equals(const Data& b) const final;
	};
}
