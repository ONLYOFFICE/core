
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Orient.h"


namespace OOX
{
	namespace Limit
	{
		Orient::Orient()
		{
			add("portrait");
			add("landscape");
		}

		const std::string Orient::no_find() const
		{
			return "portrait";
		}
	} // namespace Limit
} // namespace OOX