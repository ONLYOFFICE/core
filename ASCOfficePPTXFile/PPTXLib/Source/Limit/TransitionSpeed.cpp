#include "TransitionSpeed.h"

namespace PPTX
{
	namespace Limit
	{
		TransitionSpeed::TransitionSpeed()
		{
			add("fast");
			add("med");
			add("slow");
		}

		const std::string TransitionSpeed::no_find() const
		{
			return "fast";
		}
	} // namespace Limit
} // namespace PPTX