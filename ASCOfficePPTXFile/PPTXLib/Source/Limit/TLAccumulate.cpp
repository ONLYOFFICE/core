#include "TLAccumulate.h"

namespace PPTX
{
	namespace Limit
	{
		TLAccumulate::TLAccumulate()
		{
			add("always");
			add("none");
		}

		const std::string TLAccumulate::no_find() const
		{
			return "none";
		}
	} // namespace Limit
} // namespace PPTX