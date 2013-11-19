#include "TLOverride.h"

namespace PPTX
{
	namespace Limit
	{
		TLOverride::TLOverride()
		{
			add("childStyle");
			add("normal");
		}

		const std::string TLOverride::no_find() const
		{
			return "normal";
		}
	} // namespace Limit
} // namespace PPTX