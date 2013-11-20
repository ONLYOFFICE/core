
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "TabVal.h"


namespace OOX
{
	namespace Limit
	{
		TabVal::TabVal()
		{
			add("bar");
			add("center");
			add("clear");
			add("decimal");
			add("left");
			add("num");
			add("right");
		}

		const std::string TabVal::no_find() const
		{
			return "clear";
		}
	} // namespace Limit
} // namespace OOX