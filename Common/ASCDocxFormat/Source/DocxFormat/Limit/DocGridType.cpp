
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "DocGridType.h"


namespace OOX
{
	namespace Limit
	{
		DocGridType::DocGridType()
		{
			add("Default");
			add("lines");
			add("linesAndChars");
			add("snapToChars");
		}

		const std::string DocGridType::no_find() const
		{
			return "Default";
		}
	} // namespace Limit
} // namespace OOX