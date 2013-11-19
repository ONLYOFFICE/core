
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "TableCellType.h"


namespace Odt
{
	namespace Limit
	{
		TableCellType::TableCellType()
		{
			add("string");
		}


		const std::string TableCellType::no_find() const
		{
			return "string";
		}
	} // namespace Limit
} // namespace Odt