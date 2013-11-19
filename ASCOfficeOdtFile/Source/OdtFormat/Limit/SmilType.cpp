
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "SmilType.h"

namespace Odt
{
	namespace Limit
	{
		SmilType::SmilType()
		{
			add("barWipe");
			add("pinWheelWipe");
			add("slideWipe");
			add("randomBarWipe");
			add("checkerBoardWipe");
			add("fourBoxWipe");
			add("irisWipe");
			add("ellipseWipe");
			add("fanWipe");
			add("blindsWipe");
			add("fade");
			add("dissolve");
			add("random");
			add("pushWipe");
			add("barnDoorWipe");
			add("waterfallWipe");
		}

		const std::string SmilType::no_find() const
		{
			return "fade";
		}
	} // namespace Limit
} // namespace Odt