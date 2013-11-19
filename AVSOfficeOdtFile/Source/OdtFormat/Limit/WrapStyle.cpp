
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "WrapStyle.h"


namespace Odt
{
	namespace Limit
	{
		WrapStyle::WrapStyle()
		{
			add("none");
			add("left");
			add("right");
			add("parallel");
			add("dynamic");
			add("biggest");
			add("run-through");
		}

		const std::string WrapStyle::no_find() const
		{
			return "none";
		}
	} // namespace Limit
} // namespace Odt