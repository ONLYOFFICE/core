
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "VerticalAlign.h"


namespace OOX
{
	namespace Limit
	{
		VerticalAlign::VerticalAlign()
		{
			add("top");
			add("center");
			add("bottom");
		}

		const std::string VerticalAlign::no_find() const
		{
			return "top";
		}
	} // namespace Limit
} // namespace OOX