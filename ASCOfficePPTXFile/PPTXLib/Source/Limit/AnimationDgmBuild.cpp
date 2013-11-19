#include "AnimationDgmBuild.h"

namespace PPTX
{
	namespace Limit
	{
		AnimationDgmBuild::AnimationDgmBuild()
		{
			add("allAtOnce");
			add("lvlAtOnce");
			add("lvlOne");
			add("one");
		}

		const std::string AnimationDgmBuild::no_find() const
		{
			return "allAtOnce";
		}
	} // namespace Limit
} // namespace PPTX