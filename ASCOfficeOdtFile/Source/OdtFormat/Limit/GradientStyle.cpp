
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "GradientStyle.h"

namespace Odt
{
	namespace Limit
	{
		GradientStyle::GradientStyle()
		{
			add("radial");
			add("linear");
			add("axial");
			add("ellipsoid");
			add("square");
			add("rectangular");
		}

		const std::string GradientStyle::no_find() const
		{
			return "linear";
		}
	} // namespace Limit
} // namespace Odt