#include "IterateType.h"

namespace PPTX
{
	namespace Limit
	{
		IterateType::IterateType()
		{
			add("el");
			add("lt");
			add("wd");
		}

		const std::string IterateType::no_find() const
		{
			return "el";
		}
	} // namespace Limit
} // namespace PPTX