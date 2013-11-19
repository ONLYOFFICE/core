
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "HyphenationLadderCount.h"


namespace Odt
{
	namespace Limit
	{
		HyphenationLadderCount::HyphenationLadderCount()
		{
			add("no-limit");
		}

		const std::string HyphenationLadderCount::no_find() const
		{
			return "no-limit";
		}
	} // namespace Limit
} // namespace Odt