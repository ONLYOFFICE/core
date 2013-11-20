
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "FontCharset.h"


namespace OOX
{
	namespace Limit
	{
		FontCharset::FontCharset()
		{
			add("CC");
			add("00");
			add("02");
			add("80");
			add("86");
		}

		const std::string FontCharset::no_find() const
		{
			return "CC";
		}
	} // namespace Limit
} // namespace OOX