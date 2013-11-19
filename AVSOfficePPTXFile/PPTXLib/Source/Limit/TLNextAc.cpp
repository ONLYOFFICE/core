#include "TLNextAc.h"

namespace PPTX
{
	namespace Limit
	{
		TLNextAc::TLNextAc()
		{
			add("none");
			add("seek");
		}

		const std::string TLNextAc::no_find() const
		{
			return "none";
		}
	} // namespace Limit
} // namespace PPTX