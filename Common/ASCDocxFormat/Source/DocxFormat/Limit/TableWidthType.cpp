
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "TableWidthType.h"


namespace OOX
{
	namespace Limit
	{
		TableWidthType::TableWidthType()
		{
			add( "nil" );
            add( "auto" );
            add( "pct" );
            add( "dxa" );
		}

		const std::string TableWidthType::no_find() const
		{
			return "auto";
		}
	} // namespace Limit
} // namespace OOX