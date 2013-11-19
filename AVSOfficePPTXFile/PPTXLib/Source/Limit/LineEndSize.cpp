#include "LineEndSize.h"

namespace PPTX
{
	namespace Limit
	{
		LineEndSize::LineEndSize()
		{
			add("lg");
			add("med");
			add("sm");
		}

		const std::string LineEndSize::no_find() const
		{
			return "med";
		}
	} // namespace Limit
} // namespace PPTX