#include "SmilFill.h"

namespace Odp
{
	namespace Limit
	{
		SmilFill::SmilFill()
		{
			add("remove");
			add("freeze");
			add("hold");
			add("auto");
			add("default");
			add("transition");
		}

		const std::string SmilFill::no_find() const
		{
			return "default";
		}
	} // namespace Limit
} // namespace Odp