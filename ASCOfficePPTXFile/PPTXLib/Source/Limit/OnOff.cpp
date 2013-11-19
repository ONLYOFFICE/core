#include "OnOff.h"

namespace PPTX
{
	namespace Limit
	{
		OnOff::OnOff()
		{
			add("on");
			add("off");
			add("def");
		}

		const std::string OnOff::no_find() const
		{
			return "def";
		}
	} // namespace Limit
} // namespace PPTX