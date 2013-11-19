#include "TLNodeFillType.h"

namespace PPTX
{
	namespace Limit
	{
		TLNodeFillType::TLNodeFillType()
		{
			add("freeze");
			add("hold");
			add("remove");
			add("transition");
		}

		const std::string TLNodeFillType::no_find() const
		{
			return "hold";
		}
	} // namespace Limit
} // namespace PPTX