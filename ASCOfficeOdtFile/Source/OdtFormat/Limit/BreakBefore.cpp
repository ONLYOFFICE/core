
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "BreakBefore.h"


namespace Odt
{
	namespace Limit
	{
		BreakBefore::BreakBefore()
		{
			add("page");
			add("column");
			add("auto");
		}

		const std::string BreakBefore::no_find() const
		{
			return "auto";
		}
	} // namespace Limit
} // namespace Odt