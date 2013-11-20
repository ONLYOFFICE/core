
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "TypeFootHead.h"


namespace OOX
{
	namespace Limit
	{
		TypeFootHead::TypeFootHead()
		{
			add("even");
			add("default");
			add("first");
		}

		const std::string TypeFootHead::no_find() const
		{
			return "default";
		}
	} // namespace Limit
} // namespace OOX