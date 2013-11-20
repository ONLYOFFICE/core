
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "TableRowHeightRule.h"


namespace OOX
{
	namespace Limit
	{
		TableRowHeightRule::TableRowHeightRule()
		{
			add("auto");
  		    add("atLeast");
			add("exact");
		}

		const std::string TableRowHeightRule::no_find() const
		{
			return "auto";
		}
	} // namespace Limit
} // namespace OOX