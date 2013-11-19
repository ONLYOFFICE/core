
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "StyleClass.h"
#include <stdexcept>
#include "Exception/log_range_error.h"


namespace Odt
{
	namespace Limit
	{
		StyleClass::StyleClass()
		{
			add("text");
			add("list");
			add("extra");
			add("index");
			add("html");
			add("chapter");
		}

		const std::string StyleClass::no_find() const
		{
			throw log_range_error("style class error");
		}
	} // namespace Limit
} // namespace Odt