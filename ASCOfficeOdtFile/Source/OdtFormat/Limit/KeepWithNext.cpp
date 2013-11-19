
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "KeepWithNext.h"


namespace Odt
{
	namespace Limit
	{
		KeepWithNext::KeepWithNext()
		{
			add("always");
			add("auto");
		}

		const std::string KeepWithNext::no_find() const
		{
			return "auto";
		}
	} // namespace Limit
} // namespace Odt