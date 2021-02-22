#include "VmError.hpp"

namespace Utopia
{
	void VmError::rethrow(const SourceLocation& loc) const
	{
		impl_rethrow<VmError>(loc);
	}
}
