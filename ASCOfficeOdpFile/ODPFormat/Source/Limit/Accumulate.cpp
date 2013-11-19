#include "Accumulate.h"

namespace Odp
{
	namespace Limit
	{
		Accumulate::Accumulate()
		{
			add("sum");
			add("none");
		}

		const std::string Accumulate::no_find() const
		{
			return "none";
		}
	} // namespace Limit
} // namespace Odp