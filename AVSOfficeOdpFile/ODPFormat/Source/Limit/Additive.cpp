#include "Additive.h"

namespace Odp
{
	namespace Limit
	{
		Additive::Additive()
		{
			add("sum");
			add("replace");
		}

		const std::string Additive::no_find() const
		{
			return "replace";
		}
	} // namespace Limit
} // namespace Odp