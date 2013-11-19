#include "InOutDirectionVal.h"

namespace PPTX
{
	namespace Limit
	{
		InOutDirectionVal::InOutDirectionVal()
		{
			add("in");
			add("out");
		}

		const std::string InOutDirectionVal::no_find() const
		{
			return "out";
		}
	} // namespace Limit
} // namespace PPTX