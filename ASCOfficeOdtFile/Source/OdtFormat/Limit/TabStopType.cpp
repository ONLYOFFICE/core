
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "TabStopType.h"


namespace Odt
{
	namespace Limit
	{
		TabStopType::TabStopType()
		{
			add("right");
			add("center");
		}

		const std::string TabStopType::no_find() const
		{
			return "right";
		}
	} // namespace Limit
} // namespace Odt