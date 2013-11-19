#include "RectAlign.h"

namespace PPTX
{
	namespace Limit
	{
		RectAlign::RectAlign()
		{
			add("b");
			add("bl");
			add("br");
			add("ctr");
			add("l");
			add("r");
			add("t");
			add("tl");
			add("tr");
		}

		const std::string RectAlign::no_find() const
		{
			return "tl";
		}
	} // namespace Limit
} // namespace PPTX