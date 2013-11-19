#include "TextAlign.h"

namespace PPTX
{
	namespace Limit
	{
		TextAlign::TextAlign()
		{
			add("ctr");
			add("dist");
			add("just");
			add("justLow");
			add("l");
			add("r");
			add("thaiDist");
		}

		const std::string TextAlign::no_find() const
		{
			return "l";
		}
	} // namespace Limit
} // namespace PPTX