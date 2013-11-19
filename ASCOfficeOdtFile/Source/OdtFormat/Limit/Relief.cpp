
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Relief.h"


namespace Odt
{
	namespace Limit
	{
		Relief::Relief()
		{
			add("embossed");
			add("engraved");
			add("none");
		}

		const std::string Relief::no_find() const
		{
			return "none";
		}
	} // namespace Limit
} // namespace Odt