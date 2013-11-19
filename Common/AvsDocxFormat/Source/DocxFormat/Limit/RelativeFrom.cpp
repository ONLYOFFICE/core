
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "RelativeFrom.h"


namespace OOX
{
	namespace Limit
	{
		RelativeFrom::RelativeFrom()
		{
			add("column");
			add("paragraph");
		}

		const std::string RelativeFrom::no_find() const
		{
			return "column";
		}
	} // namespace Limit
} // namespace OOX