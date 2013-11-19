#include "CompoundLine.h"

namespace PPTX
{
	namespace Limit
	{
		CompoundLine::CompoundLine()
		{
			add("dbl");
			add("sng");
			add("thickThin");
			add("thinThick");
			add("tri");
		}

		const std::string CompoundLine::no_find() const
		{
			return "sng";
		}
	} // namespace Limit
} // namespace PPTX