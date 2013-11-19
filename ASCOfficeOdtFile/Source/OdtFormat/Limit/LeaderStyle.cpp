
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "LeaderStyle.h"


namespace Odt
{
	namespace Limit
	{
		LeaderStyle::LeaderStyle()
		{
			add("dotted");
		}

		const std::string LeaderStyle::no_find() const
		{
			return "dotted";
		}
	} // namespace Limit
} // namespace Odt