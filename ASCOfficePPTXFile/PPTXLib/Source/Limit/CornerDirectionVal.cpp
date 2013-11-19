#include "CornerDirectionVal.h"

namespace PPTX
{
	namespace Limit
	{
		CornerDirectionVal::CornerDirectionVal()
		{
			add("ld");
			add("lu");
			add("rd");
			add("ru");
		}

		const std::string CornerDirectionVal::no_find() const
		{
			return "lu";
		}
	} // namespace Limit
} // namespace PPTX