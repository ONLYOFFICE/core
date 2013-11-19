
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "UnderType.h"


namespace OOX
{
	namespace Limit
	{
		UnderType::UnderType()
		{
			add("single");
			add("words");
			add("double");
			add("thick");
			add("dotted");
			add("dottedHeavy");
			add("dash");
			add("dashedHeavy");
			add("dashLong");
			add("dashLongHeavy");
			add("dotDash");
			add("dashDotHeavy");
			add("dotDotDash");
			add("dashDotDotHeavy");
			add("wave");
			add("wavyHeavy");
			add("wavyDouble");
			add("none");
		}

		const std::string UnderType::no_find() const
		{
			return "none";
		}
	} // namespace Limit
} // namespace OOX