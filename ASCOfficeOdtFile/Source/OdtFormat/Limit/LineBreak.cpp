
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "LineBreak.h"


namespace Odt
{
	namespace Limit
	{
		LineBreak::LineBreak()
		{
			add("strict");
		}

		const std::string LineBreak::no_find() const
		{
			return "strict";
		}
	} // namespace Limit
} // namespace Odt