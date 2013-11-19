#include "TLMasterRelation.h"

namespace PPTX
{
	namespace Limit
	{
		TLMasterRelation::TLMasterRelation()
		{
			add("lastClick");
			add("nextClick");
			add("sameClick");
		}

		const std::string TLMasterRelation::no_find() const
		{
			return "sameClick";
		}
	} // namespace Limit
} // namespace PPTX