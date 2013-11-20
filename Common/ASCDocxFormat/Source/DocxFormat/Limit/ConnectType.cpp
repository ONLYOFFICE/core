
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "ConnectType.h"


namespace OOX
{
	namespace Limit
	{
		ConnectType::ConnectType()
		{
			add("rect");
			add("custom");
		}

		const std::string ConnectType::no_find() const
		{
			return "rect";
		}
	} // namespace Limit
} // namespace OOX