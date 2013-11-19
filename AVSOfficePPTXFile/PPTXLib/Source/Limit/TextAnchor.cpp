#include "TextAnchor.h"

namespace PPTX
{
	namespace Limit
	{
		TextAnchor::TextAnchor()
		{
			add("b");
			add("ctr");
			add("dist");
			add("just");
			add("t");
		}

		const std::string TextAnchor::no_find() const
		{
			return "t";
		}
	} // namespace Limit
} // namespace PPTX