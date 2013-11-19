
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "TextAlign.h"


namespace Odt
{
	namespace Limit
	{
		TextAlign::TextAlign()
		{
			add("start");
			add("center");
			add("end");
			add("justify");
		}

		const std::string TextAlign::no_find() const
		{
			return "start";
		}
	} // namespace Limit
} // namespace Odt