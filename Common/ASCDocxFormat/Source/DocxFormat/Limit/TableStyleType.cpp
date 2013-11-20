
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "TableStyleType.h"


namespace OOX
{
	namespace Limit
	{
		TableStyleType::TableStyleType()
		{
			add("band1Horz");
			add("band1Vert");
			add("band2Horz");
			add("band2Vert");
			add("firstCol");
			add("firstRow");
			add("lastCol");
			add("lastRow");
			add("neCell");
			add("nwCell");
			add("seCell");
			add("swCell");
			add("wholeTable");
		}

		const std::string TableStyleType::no_find() const
		{
			return "wholeTable";
		}
	} // namespace Limit
} // namespace OOX