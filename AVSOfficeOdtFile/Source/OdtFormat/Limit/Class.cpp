
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Class.h"

namespace Odt
{
	namespace Limit
	{
		Class::Class()
		{
			add("title");
			add("outline");
			add("date-time");
			add("header");
			add("footer");
			add("page-number");
			add("page");
			add("notes");
			add("subtitle");
			add("graphic");
			add("object");
			add("chart");
		}

		const std::string Class::no_find() const
		{
			return "object";
		}
	} // namespace Limit
} // namespace Odt