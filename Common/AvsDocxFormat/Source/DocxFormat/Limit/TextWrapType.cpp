
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "TextWrapType.h"


namespace OOX
{
	namespace Limit
	{
		TextWrapType::TextWrapType()
		{
			add("bothSides");
			add("left");
			add("right");
			add("largest");
		}

		const std::string TextWrapType::no_find() const
		{
			return "bothSides";
		}
	} // namespace Limit
} // namespace OOX