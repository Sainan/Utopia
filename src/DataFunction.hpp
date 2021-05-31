#pragma once

#include "Data.hpp"

#include "Scope.hpp"

namespace Utopia
{
	class DataFunction : public Data
	{
	public:
		Scope scope;

		explicit DataFunction(Scope&& scope = {});

		[[nodiscard]] std::string toString() const final;
		[[nodiscard]] std::string toCPP() const final;

		[[nodiscard]] std::unique_ptr<Data> copy() const final;

		[[nodiscard]] bool equals(const Data& b) const final;
	};
}
