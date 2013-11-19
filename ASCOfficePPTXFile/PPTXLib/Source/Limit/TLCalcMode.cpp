#include "TLCalcMode.h"

namespace PPTX
{
	namespace Limit
	{
		TLCalcMode::TLCalcMode()
		{
			add("lin");
			add("fmla");
			add("discrete");
		}

		const std::string TLCalcMode::no_find() const
		{
			return "lin";
		}
	} // namespace Limit
} // namespace PPTX