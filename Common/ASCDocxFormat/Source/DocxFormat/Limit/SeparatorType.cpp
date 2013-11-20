
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "SeparatorType.h"


namespace OOX
{
	namespace Limit
	{
		SeparatorType::SeparatorType()
		{
			add("separator");
			add("continuationSeparator");
		}

		const std::string SeparatorType::no_find() const
		{
			return "separator";
		}
	} // namespace Limit
} // namespace OOX