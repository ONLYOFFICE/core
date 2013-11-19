#include "SmilMode.h"

namespace Odp
{
	namespace Limit
	{
		SmilMode::SmilMode()
		{
			add("in");
			add("out");
		}

		const std::string SmilMode::no_find() const
		{
			return "out";
		}
	} // namespace Limit
} // namespace Odp