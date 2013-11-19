#include "FrameShape.h"

namespace PPTX
{
	namespace Limit
	{
		FrameShape::FrameShape()
		{
			add("frameStyle1");
			add("frameStyle2");
			add("frameStyle3");
			add("frameStyle4");
			add("frameStyle5");
			add("frameStyle6");
			add("frameStyle7");
		}

		const std::string FrameShape::no_find() const
		{
			return "frameStyle1";
		}
	} // namespace Limit
} // namespace PPTX