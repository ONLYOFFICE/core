
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "HorizontalAnchorLocation.h"


namespace OOX
{
	namespace Limit
	{
		HorizontalAnchorLocation::HorizontalAnchorLocation()
		{
			add("margin");
			add("page");
			add("text");
			add("none");
		}

	    const std::string HorizontalAnchorLocation::no_find() const
		{
			return "text";
		}
	} // namespace Limit
} // namespace OOX