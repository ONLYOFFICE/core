
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "TextAlignment.h"


namespace OOX
{
	namespace Limit
	{
		TextAlignment::TextAlignment()
		{
			add("baseline");
		}

		const std::string TextAlignment::no_find() const
		{
			return "none";
		}
	} // namespace Limit
} // namespace OOX