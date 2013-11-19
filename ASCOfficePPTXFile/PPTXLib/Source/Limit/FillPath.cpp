#include "FillPath.h"

namespace PPTX
{
	namespace Limit
	{
		FillPath::FillPath()
		{
			add("circle");
			add("rect");
			add("shape");
		}

		const std::string FillPath::no_find() const
		{
			return "circle";
		}
	} // namespace Limit
} // namespace PPTX