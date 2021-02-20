#pragma once

#include "fwddecl.hpp"

#include <memory>
#include <string>

namespace Utopia
{
	enum DataType
	{
		DATA_EMPTY,
		DATA_STRING,
		DATA_INT,
		DATA_FUNC,
	};

	class Data
	{
	public:
		const DataType type;

	protected:
		explicit Data(const DataType type);

	public:
		virtual ~Data();

		[[nodiscard]] static const char* getTypeName(const DataType type);
	private:
		[[nodiscard]] std::string getTypeErrorMessage(const DataType expected_type) const;
	public:
		void expectType(const DataType expected_type) const;
		void expectType(const DataType expected_type, const SourceLocation& loc) const;

		[[nodiscard]] virtual std::string toString() const = 0;
		[[nodiscard]] virtual std::string toCPP() const = 0;

		[[nodiscard]] virtual std::unique_ptr<Data> copy() const = 0;
	};
}
