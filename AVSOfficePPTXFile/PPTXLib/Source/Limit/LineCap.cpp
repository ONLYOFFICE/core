#include "LineCap.h"

namespace PPTX
{
	namespace Limit
	{
		LineCap::LineCap()
		{
			add("flat");
			add("rnd");
			add("sq");
		}

		const std::string LineCap::no_find() const
		{
			return "flat";
		}
	} // namespace Limit
} // namespace PPTX