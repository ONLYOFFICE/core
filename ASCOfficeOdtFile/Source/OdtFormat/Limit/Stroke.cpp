
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Stroke.h"


namespace Odt
{
	namespace Limit
	{
		Stroke::Stroke()
		{
			add("solid");
			add("none");
		}

		const std::string Stroke::no_find() const
		{
			return "solid";
		}
	} // namespace Limit
} // namespace Odt