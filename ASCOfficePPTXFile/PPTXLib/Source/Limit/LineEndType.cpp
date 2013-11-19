#include "LineEndType.h"

namespace PPTX
{
	namespace Limit
	{
		LineEndType::LineEndType()
		{
			add("none");
			add("arrow");
			add("diamond");
			add("oval");
			add("stealth");
			add("triangle");
		}

		const std::string LineEndType::no_find() const
		{
			return "none";
		}
	} // namespace Limit
} // namespace PPTX