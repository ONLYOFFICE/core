#include "SideDirectionVal.h"

namespace PPTX
{
	namespace Limit
	{
		SideDirectionVal::SideDirectionVal()
		{
			add("d");
			add("l");
			add("r");
			add("u");
		}

		const std::string SideDirectionVal::no_find() const
		{
			return "l";
		}
	} // namespace Limit
} // namespace PPTX