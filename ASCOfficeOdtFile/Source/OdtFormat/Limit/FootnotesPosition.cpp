
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "FootnotesPosition.h"


namespace Odt
{
	namespace Limit
	{
		FootnotesPosition::FootnotesPosition()
		{
			add("page");
			add("document");
		}

		const std::string FootnotesPosition::no_find() const
		{
			return "page";
		}
	} // namespace Limit
} // namespace Odt