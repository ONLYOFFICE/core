
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "TextareaAlign.h"


namespace Odt
{
	namespace Limit
	{
		TextareaAlign::TextareaAlign()
		{
			add("center");
			add("middle");
			add("left");
			add("right");
			add("top");
			add("bottom");
			add("justify");
		}

		const std::string TextareaAlign::no_find() const
		{
			return "center";
		}
	} // namespace Limit
} // namespace Odt