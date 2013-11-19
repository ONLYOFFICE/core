
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Rel.h"


namespace Odt
{
	namespace Limit
	{
		Rel::Rel()
		{
			add("paragraph");
			add("page");
			add("frame");
			add("paragraph-content");
			add("page-content");
			add("frame-content");
			add("line");
			add("baseline");
			add("char");
			add("text");
		}

		const std::string Rel::no_find() const
		{
			return "paragraph";
		}
	} // namespace Limit
} // namespace Odt