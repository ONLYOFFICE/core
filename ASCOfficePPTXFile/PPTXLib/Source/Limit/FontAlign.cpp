#include "FontAlign.h"

namespace PPTX
{
	namespace Limit
	{
		FontAlign::FontAlign()
		{
			add("auto");
			add("b");
			add("base");
			add("ctr");
			add("t");
		}

		const std::string FontAlign::no_find() const
		{
			return "auto";
		}
	} // namespace Limit
} // namespace PPTX