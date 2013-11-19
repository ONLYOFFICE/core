#include "BevelType.h"

namespace PPTX
{
	namespace Limit
	{
		BevelType::BevelType()
		{
			add("angle");
			add("artDeco");
			add("circle");
			add("convex");
			add("coolSlant");
			add("cross");
			add("divot");
			add("hardEdge");
			add("relaxedInset");
			add("riblet");
			add("slope");
			add("softRound");
		}

		const std::string BevelType::no_find() const
		{
			return "circle";
		}
	} // namespace Limit
} // namespace PPTX