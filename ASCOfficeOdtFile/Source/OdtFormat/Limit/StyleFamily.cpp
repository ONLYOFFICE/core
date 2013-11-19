
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "StyleFamily.h"
#include <stdexcept>
#include "Exception/log_range_error.h"


namespace Odt
{
	namespace Limit
	{
		StyleFamily::StyleFamily()
		{
			add("table");
			add("table-column");
			add("table-row");
			add("table-cell");
			add("paragraph");
			add("text");
			add("graphic");
			add("section");
			add("presentation");
			add("drawing-page");
		}

		const std::string StyleFamily::no_find() const
		{
			return "paragraph";
		}
	} // namespace Limit
} // namespace Odt