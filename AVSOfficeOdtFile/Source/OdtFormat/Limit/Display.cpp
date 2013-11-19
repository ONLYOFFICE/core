
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Display.h"


namespace Odt
{
	namespace Limit
	{
		Display::Display()
		{
			add("true");
			add("none");
		}

		const std::string Display::no_find() const
		{
			return "true";
		}
	} // namespace Limit
} // namespace Odt