
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Position.h"


namespace Odt
{
	namespace Limit
	{
		Position::Position()
		{
			add("from-top");
			add("from-bottom");
			add("from-center");
			add("from-left");
			add("from-right");
			add("top");
			add("bottom");
			add("center");
			add("left");
			add("right");
			add("middle");
			add("below");

			add("center center");
			add("top center");
			add("top right");
			add("center right");
			add("bottom right");
			add("bottom center");
			add("bottom left");
			add("center left");
			add("top left");
		}

		const std::string Position::no_find() const
		{
			return "from-top";
		}
	} // namespace Limit
} // namespace Odt