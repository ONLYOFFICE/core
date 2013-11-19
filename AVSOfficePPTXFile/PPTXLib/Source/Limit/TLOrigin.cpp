#include "TLOrigin.h"

namespace PPTX
{
	namespace Limit
	{
		TLOrigin::TLOrigin()
		{
			add("parent");
			add("layout");
		}

		const std::string TLOrigin::no_find() const
		{
			return "parent";
		}
	} // namespace Limit
} // namespace PPTX