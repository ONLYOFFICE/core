
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Anchortype.h"


namespace Odt
{
	namespace Limit
	{
		Anchortype::Anchortype()
		{
			add("paragraph");
			add("aschar");
			add("as-char");
			add("page");
			add("frame");
			add("char");
		}

		const std::string Anchortype::no_find() const
		{
			return "paragraph";
		}
	} // namespace Limit
} // namespace Odt