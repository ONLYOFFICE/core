#include "PresetClass.h"

namespace Odp
{
	namespace Limit
	{
		PresetClass::PresetClass()
		{
			add("custom");
			add("entrance");
			add("exit");
			add("emphasis");
			add("motion-path");
			add("ole-action");
			add("media-call");
		}

		const std::string PresetClass::no_find() const
		{
			return "custom";
		}
	} // namespace Limit
} // namespace Odp