#include "TLNodeType.h"

namespace PPTX
{
	namespace Limit
	{
		TLNodeType::TLNodeType()
		{
			add("afterEffect");
			add("afterGroup");
			add("clickEffect");
			add("clickPar");
			add("interactiveSeq");
			add("mainSeq");
			add("tmRoot");
			add("withEffect");
			add("withGroup");
		}

		const std::string TLNodeType::no_find() const
		{
			return "tmRoot";
		}
	} // namespace Limit
} // namespace PPTX