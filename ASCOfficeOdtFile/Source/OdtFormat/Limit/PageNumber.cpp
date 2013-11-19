
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "PageNumber.h"


namespace Odt
{
	namespace Limit
	{
		PageNumber::PageNumber()
		{
			add("auto");
		}

		const std::string PageNumber::no_find() const
		{
			return "auto";
		}
	} // namespace Limit
} // namespace Odt