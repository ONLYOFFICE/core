#include "EffectContainerType.h"

namespace PPTX
{
	namespace Limit
	{
		EffectContainerType::EffectContainerType()
		{
			add("sib");
			add("tree");
		}

		const std::string EffectContainerType::no_find() const
		{
			return "sib";
		}
	} // namespace Limit
} // namespace PPTX