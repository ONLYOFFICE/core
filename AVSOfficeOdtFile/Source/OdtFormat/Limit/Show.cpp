
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Show.h"


namespace Odt
{
	namespace Limit
	{
		Show::Show()
		{
			add("embed");
		}

		const std::string Show::no_find() const
		{
			return "embed";
		}
	} // namespace Limit
} // namespace Odt