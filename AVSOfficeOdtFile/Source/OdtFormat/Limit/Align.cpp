
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Align.h"


namespace Odt
{
	namespace Limit
	{
		Align::Align()
		{
			add("margins");
			add("left");
			add("right");
			add("top");
			add("bottom");
		}

		const std::string Align::no_find() const
		{
			return "margins";
		}
	} // namespace Limit
} // namespace Odt