#include "Orient.h"

namespace PPTX
{
	namespace Limit
	{
		Orient::Orient()
		{
			add("horz");
			add("vert");
		}

		const std::string Orient::no_find() const
		{
			return "vert";
		}
	} // namespace Limit
} // namespace PPTX