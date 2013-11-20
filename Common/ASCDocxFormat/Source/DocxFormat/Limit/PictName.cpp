
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "PictName.h"


namespace OOX
{
	namespace Limit
	{

		PictName::PictName()
		{
			add("pict");
			add("object");
		}


		const std::string PictName::no_find() const
		{
			return "pict";
		}

	} // namespace Limit
} // namespace OOX