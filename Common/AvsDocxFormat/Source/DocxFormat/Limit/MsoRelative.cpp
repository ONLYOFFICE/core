
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "MsoRelative.h"


namespace OOX
{
	namespace Limit
	{
		MsoRelative::MsoRelative()
		{
 			add("margin");
            add("page");
		}

		const std::string MsoRelative::no_find() const
		{
			return "margin";
		}
	} // namespace Limit
} // namespace OOX