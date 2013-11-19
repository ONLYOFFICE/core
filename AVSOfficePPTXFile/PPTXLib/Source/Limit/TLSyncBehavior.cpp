#include "TLSyncBehavior.h"

namespace PPTX
{
	namespace Limit
	{
		TLSyncBehavior::TLSyncBehavior()
		{
			add("canSlip");
			add("locked");
		}

		const std::string TLSyncBehavior::no_find() const
		{
			return "canSlip";
		}
	} // namespace Limit
} // namespace PPTX