#include "TLPrevAc.h"

namespace PPTX
{
	namespace Limit
	{
		TLPrevAc::TLPrevAc()
		{
			add("none");
			add("skipTimed");
		}

		const std::string TLPrevAc::no_find() const
		{
			return "none";
		}
	} // namespace Limit
} // namespace PPTX