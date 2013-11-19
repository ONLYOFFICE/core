#include "PresetShadowVal.h"

namespace PPTX
{
	namespace Limit
	{
		PresetShadowVal::PresetShadowVal()
		{
			add("shdw1");
			add("shdw2");
			add("shdw3");
			add("shdw4");
			add("shdw5");
			add("shdw6");
			add("shdw7");
			add("shdw8");
			add("shdw9");
			add("shdw10");
			add("shdw11");
			add("shdw12");
			add("shdw13");
			add("shdw14");
			add("shdw15");
			add("shdw16");
			add("shdw17");
			add("shdw18");
			add("shdw19");
			add("shdw20");
		}

		const std::string PresetShadowVal::no_find() const
		{
			return "shdw1";
		}
	} // namespace Limit
} // namespace PPTX