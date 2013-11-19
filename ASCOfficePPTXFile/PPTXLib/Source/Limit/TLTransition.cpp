#include "TLTransition.h"

namespace PPTX
{
	namespace Limit
	{
		TLTransition::TLTransition()
		{
			add("in");
			add("out");
			add("none");			
		}

		const std::string TLTransition::no_find() const
		{
			return "none";
		}
	} // namespace Limit
} // namespace PPTX