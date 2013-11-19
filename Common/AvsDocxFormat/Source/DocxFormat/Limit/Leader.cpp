
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Leader.h"


namespace OOX
{
	namespace Limit
	{
		Leader::Leader()
		{
			add("dot");
			add("heavy");
			add("hyphen");
			add("middleDot");
			add("none");
			add("underscore");
		}

		const std::string Leader::no_find() const
		{
			return "none";
		}
	} // namespace Limit
} // namespace OOX