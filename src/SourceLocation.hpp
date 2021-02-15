#pragma once

#include <string>

#include "Shared.hpp"

namespace Utopia
{
	struct SourceLocation
	{
		Shared<std::string> name;
		size_t line = 1;
		size_t character = 1;
		
		explicit SourceLocation(std::string&& name);

		void newline();

		[[nodiscard]] std::string getSuffix() const;

		template <typename E>
		__declspec(noreturn) void throwHere(std::string&& error) const
		{
			throw E(error.append(getSuffix()));
		}
	};
}
