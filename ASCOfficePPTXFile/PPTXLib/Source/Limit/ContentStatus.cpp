#include "ContentStatus.h"


namespace PPTX
{
	namespace Limit
	{
		ContentStatus::ContentStatus()
		{
			add("Draft");
			add("Reviewed");
			add("Final");
		}

		const std::string ContentStatus::no_find() const
		{
			return "Draft";
		}
	} // namespace Limit
} // namespace PPTX