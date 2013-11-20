
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Suffix.h"


namespace OOX
{
	namespace Limit
	{
		Suffix::Suffix()
		{
			add("tab");
			add("space");
			add("nothing");
		}

		const std::string Suffix::no_find() const
		{
			return "nothing";
		}
	} // namespace Limit
} // namespace OOX