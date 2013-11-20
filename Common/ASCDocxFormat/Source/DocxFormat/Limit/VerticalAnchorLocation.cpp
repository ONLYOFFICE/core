
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "VerticalAnchorLocation.h"


namespace OOX
{
	namespace Limit
	{
		VerticalAnchorLocation::VerticalAnchorLocation()
		{
		    add("margin");
			add("page");
			add("text");
			add("none");
		}

		const std::string VerticalAnchorLocation::no_find() const
		{
		    return "text";
		}
	} // namespace Limit
} // namespace OOX