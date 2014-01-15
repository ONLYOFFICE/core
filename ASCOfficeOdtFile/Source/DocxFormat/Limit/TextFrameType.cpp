
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "TextFrameType.h"


namespace OOX
{
	namespace Limit
	{
		TextFrameType::TextFrameType()
		{
			add("auto");
			add("notBeside");
			add("around");
			add("none");
			add("tight");
			add("through");
		}

		const std::string TextFrameType::no_find() const
		{
			return "none";
		}
	} // namespace Limit
} // namespace OOX