#include "SmilFillDefault.h"

namespace Odp
{
	namespace Limit
	{
		SmilFillDefault::SmilFillDefault()
		{
			add("remove");
			add("freeze");
			add("hold");
			add("transition");
			add("auto");
			add("inherit");
		}

		const std::string SmilFillDefault::no_find() const
		{
			return "inherit";
		}
	} // namespace Limit
} // namespace Odp