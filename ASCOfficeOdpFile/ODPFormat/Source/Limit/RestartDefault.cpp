#include "RestartDefault.h"

namespace Odp
{
	namespace Limit
	{
		RestartDefault::RestartDefault()
		{
			add("never");
			add("always");
			add("whenNotActive");
			add("inherit");
		}

		const std::string RestartDefault::no_find() const
		{
			return "inherit";
		}
	} // namespace Limit
} // namespace Odp