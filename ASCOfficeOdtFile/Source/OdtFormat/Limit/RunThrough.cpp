
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "RunThrough.h"


namespace Odt
{
	namespace Limit
	{
		RunThrough::RunThrough()
		{
			add("foreground");
			add("background");
		}

		const std::string RunThrough::no_find() const
		{
			return "foreground";
		}
	} // namespace Limit
} // namespace Odt