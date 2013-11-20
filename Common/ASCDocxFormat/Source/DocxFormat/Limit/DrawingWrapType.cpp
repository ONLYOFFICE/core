
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "DrawingWrapType.h"


namespace OOX
{
	namespace Limit
	{
		DrawingWrapType::DrawingWrapType()
		{
			add("wrapNone");
			add("wrapSquare");
			add("wrapTight");
			add("wrapThrough");
			add("wrapTopAndBottom");
		}

		const std::string DrawingWrapType::no_find() const
		{
			return "wrapTopAndBottom";
		}
	} // namespace Limit
} // namespace OOX