
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "DropCap.h"


namespace OOX
{
	namespace Limit
	{
		DropCap::DropCap()
		{
			add("drop");
			add("margin");
			add("none");
		}

		const std::string DropCap::no_find() const
		{
			return "none";
		}
	} // namespace Limit
} // namespace OOX