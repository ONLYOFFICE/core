#include "TLRuntimeTrigger.h"

namespace PPTX
{
	namespace Limit
	{
		TLRuntimeTrigger::TLRuntimeTrigger()
		{
			add("all");
			add("first");
			add("last");			
		}

		const std::string TLRuntimeTrigger::no_find() const
		{
			return "first";
		}
	} // namespace Limit
} // namespace PPTX