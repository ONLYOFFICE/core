
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "SelectPage.h"


namespace Odt
{
	namespace Limit
	{
		SelectPage::SelectPage()
		{
			add("current");
		}

		const std::string SelectPage::no_find() const
		{
			return "current";
		}
	} // namespace Limit
} // namespace Odt