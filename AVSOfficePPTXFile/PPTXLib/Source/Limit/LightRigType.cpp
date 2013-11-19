#include "LightRigType.h"

namespace PPTX
{
	namespace Limit
	{
		LightRigType::LightRigType()
		{
			add("balanced");
			add("brightRoom");
			add("chilly");
			add("contrasting");
			add("flat");
			add("flood");
			add("freezing");
			add("glow");
			add("harsh");
			add("legacyFlat1");
			add("legacyFlat2");
			add("legacyFlat3");
			add("legacyFlat4");
			add("legacyHarsh1");
			add("legacyHarsh2");
			add("legacyHarsh3");
			add("legacyHarsh4");
			add("legacyNormal1");
			add("legacyNormal2");
			add("legacyNormal3");
			add("legacyNormal4");
			add("morning");
			add("soft");
			add("sunrise");
			add("sunset");
			add("threePt");
			add("twoPt");
		}

		const std::string LightRigType::no_find() const
		{
			return "balanced";
		}
	} // namespace Limit
} // namespace PPTX