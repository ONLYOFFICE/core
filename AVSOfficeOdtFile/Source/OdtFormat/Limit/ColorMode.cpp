
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "ColorMode.h"


namespace Odt
{
	namespace Limit
	{
		ColorMode::ColorMode()
		{
			add("standard");
		}

		const std::string ColorMode::no_find() const
		{
			return "standard";
		}
	} // namespace Limit
} // namespace Odt