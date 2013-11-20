
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Join.h"


namespace OOX
{
	namespace Limit
	{
		Join::Join()
		{
			add("miter");
		}

		const std::string Join::no_find() const
		{
			return "miter";
		}
	} // namespace Limit
} // namespace OOX