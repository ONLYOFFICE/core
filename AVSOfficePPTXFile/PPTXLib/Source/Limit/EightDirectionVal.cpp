#include "EightDirectionVal.h"

namespace PPTX
{
	namespace Limit
	{
		EightDirectionVal::EightDirectionVal()
		{
			add("ld");
			add("lu");
			add("rd");
			add("ru");
			add("d");
			add("l");
			add("r");
			add("u");
		}

		const std::string EightDirectionVal::no_find() const
		{
			return "l";
		}
	} // namespace Limit
} // namespace PPTX