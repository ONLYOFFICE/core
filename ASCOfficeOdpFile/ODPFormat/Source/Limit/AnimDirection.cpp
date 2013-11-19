#include "AnimDirection.h"

namespace Odp
{
	namespace Limit
	{
		AnimDirection::AnimDirection()
		{
			add("forward");
			add("reverse");
		}

		const std::string AnimDirection::no_find() const
		{
			return "forward";
		}
	} // namespace Limit
} // namespace Odp