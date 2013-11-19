#include "Restart.h"

namespace Odp
{
	namespace Limit
	{
		Restart::Restart()
		{
			add("never");
			add("always");
			add("whenNotActive");
			add("default");
		}

		const std::string Restart::no_find() const
		{
			return "default";
		}
	} // namespace Limit
} // namespace Odp