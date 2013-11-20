
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "LineRule.h"


namespace OOX
{
	namespace Limit
	{
		LineRule::LineRule()
		{
			add("auto");
			add("atLeast");
			add("exact");
			add("exactly");
		}

		const std::string LineRule::no_find() const
		{
			return "auto";
		}
	} // namespace Limit
} // namespace OOX