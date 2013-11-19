#include "TextCaps.h"

namespace PPTX
{
	namespace Limit
	{
		TextCaps::TextCaps()
		{
			add("all");
			add("none");
			add("small");
		}

		const std::string TextCaps::no_find() const
		{
			return "none";
		}
	} // namespace Limit
} // namespace PPTX