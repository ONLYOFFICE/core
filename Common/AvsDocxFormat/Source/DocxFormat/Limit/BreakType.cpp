
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "BreakType.h"


namespace OOX
{
	namespace Limit
	{
		BreakType::BreakType()
		{
			add("page");
			add("column");
			add("textWrapping");
		}

		const std::string BreakType::no_find() const
		{
			return "page";
		}
	} // namespace Limit
} // namespace OOX