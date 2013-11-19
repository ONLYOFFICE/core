#include "VertOverflow.h"

namespace PPTX
{
	namespace Limit
	{
		VertOverflow::VertOverflow()
		{
			add("clip");
			add("ellipsis");
			add("overflow");
		}

		const std::string VertOverflow::no_find() const
		{
			return "clip";
		}
	} // namespace Limit
} // namespace PPTX