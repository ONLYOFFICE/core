#include "PenAlign.h"

namespace PPTX
{
	namespace Limit
	{
		PenAlign::PenAlign()
		{
			add("ctr");
			add("in");
		}

		const std::string PenAlign::no_find() const
		{
			return "ctr";
		}
	} // namespace Limit
} // namespace PPTX