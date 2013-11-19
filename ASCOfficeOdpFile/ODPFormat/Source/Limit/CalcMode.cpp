#include "CalcMode.h"

namespace Odp
{
	namespace Limit
	{
		CalcMode::CalcMode()
		{
			add("discrete");
			add("linear");
			add("paced");
			add("spline");
		}

		const std::string CalcMode::no_find() const
		{
			return "discrete";
		}
	} // namespace Limit
} // namespace Odp