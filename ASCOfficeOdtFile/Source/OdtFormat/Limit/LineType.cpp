
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "LineType.h"


namespace Odt
{
	namespace Limit
	{
		LineType::LineType()
		{
			add("none");
			add("single");
			add("double");
		}

		const std::string LineType::no_find() const
		{
			return "single";
		}
	} // namespace Limit
} // namespace Odt