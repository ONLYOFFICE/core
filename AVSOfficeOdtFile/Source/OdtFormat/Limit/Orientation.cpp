
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Orientation.h"


namespace Odt
{
	namespace Limit
	{
		Orientation::Orientation()
		{
			add("portrait");
			add("landscape");
		}

		const std::string Orientation::no_find() const
		{
			return "portrait";
		}
	} // namespace Limit
} // namespace Odt