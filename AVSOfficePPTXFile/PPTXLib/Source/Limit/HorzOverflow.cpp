#include "HorzOverflow.h"

namespace PPTX
{
	namespace Limit
	{
		HorzOverflow::HorzOverflow()
		{
			add("clip");
			add("overflow");
		}

		const std::string HorzOverflow::no_find() const
		{
			return "clip";
		}
	} // namespace Limit
} // namespace PPTX