
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "LineStyle.h"


namespace Odt
{
	namespace Limit
	{
		LineStyle::LineStyle()
		{
			add("none");
			add("solid");
			add("dotted");
			add("dash");
			add("long-dash");
			add("dot-dash");
			add("dot-dot-dash");
			add("wave");
		}

		const std::string LineStyle::no_find() const
		{
			return "none";
		}
	} // namespace Limit
} // namespace Odt