#pragma once

#include "fwddecl.hpp"

#include <memory>
#include <string>

#include "TracableError.hpp"

namespace Utopia
{
	struct SourceLocation
	{
		const std::shared_ptr<std::string> name;
		size_t line = 1;
		size_t character = 1;
		
		explicit SourceLocation(std::string&& name);

		void newline();

		[[nodiscard]] std::string toString() const;
		[[nodiscard]] std::string getSuffix() const;

		template <class E>
		[[noreturn]] void throwHere(std::string&& error) const
		{
			if constexpr (std::is_base_of_v<TracableError, E>)
			{
				throw E(error.append(getSuffix()), true);
			}
			else
			{
				throw E(error.append(getSuffix()));
			}
		}
	};
}
