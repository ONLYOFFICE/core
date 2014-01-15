
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "FontFamily.h"


namespace OOX
{
	namespace Limit
	{
		FontFamily::FontFamily()
		{
			add("swiss");
			add("roman");
			add("decorative");
			add("auto");
			add("script");
			add("modern");
		}

		const std::string FontFamily::no_find() const
		{
			return "swiss";
		}
	} // namespace Limit
} // namespace OOX