#pragma once

#include "TracableError.hpp"

#include "SourceLocation.hpp"

namespace Utopia
{
	class VmError : public TracableError
	{
	public:
		using TracableError::TracableError;

	protected:
		template <class E>
		[[noreturn]] void impl_rethrow(const SourceLocation& loc) const
		{
			std::string reason(what());
			if (got_loc)
			{
				reason.append(" while executing function");
			}
			reason.append(loc.getSuffix());
			throw E(reason, true);
		}

	public:
		[[noreturn]] virtual void rethrow(const SourceLocation& loc) const;
	};
}
