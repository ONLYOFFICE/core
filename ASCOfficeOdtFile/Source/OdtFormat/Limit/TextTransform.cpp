
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "TextTransform.h"


namespace Odt
{
	namespace Limit
	{
		TextTransform::TextTransform()
		{
			add("none");
			add("normal");
			add("uppercase");			
			add("lowercase");			
			add("capitalize");
		}

		const std::string TextTransform::no_find() const
		{
			return "normal";
		}
	} // namespace Limit
} // namespace Odt