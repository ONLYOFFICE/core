
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Fill.h"


namespace Odt
{
	namespace Limit
	{
		Fill::Fill()
		{
			add("solid");
			add("bitmap");
			add("gradient");
			add("none");
		}

		const std::string Fill::no_find() const
		{
			return "solid";
		}
	} // namespace Limit
} // namespace Odt