#pragma once

#include <memory>
#include <string>

namespace Utopia
{
	enum DataType
	{
		DATA_EMPTY,
		DATA_STRING,
		DATA_INT,
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
		void expectType(const DataType expected_type) const;

		[[nodiscard]] virtual std::string toString() const = 0;
		[[nodiscard]] virtual std::string toCPP() const = 0;

		[[nodiscard]] virtual std::unique_ptr<Data> copy() const = 0;
	};
}
