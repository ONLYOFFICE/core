
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "BreakClear.h"


namespace OOX
{
	namespace Limit
	{
		BreakClear::BreakClear()
		{
			add("all");
			add("left");
			add("none");
			add("right");
		}

		const std::string BreakClear::no_find() const
		{
			return "all";
		}
	} // namespace Limit
} // namespace OOX