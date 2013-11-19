#include "PlaceholderSize.h"

namespace PPTX
{
	namespace Limit
	{
		PlaceholderSize::PlaceholderSize()
		{
			add("full");
			add("half");
			add("quarter");
		}

		const std::string PlaceholderSize::no_find() const
		{
			return "full";
		}
	} // namespace Limit
} // namespace PPTX