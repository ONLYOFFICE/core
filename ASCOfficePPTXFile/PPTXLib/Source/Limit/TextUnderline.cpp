#include "TextUnderline.h"

namespace PPTX
{
	namespace Limit
	{
		TextUnderline::TextUnderline()
		{
			add("dash");
			add("dashHeavy");
			add("dashLong");
			add("dashLongHeavy");
			add("dbl");
			add("dotDash");
			add("dotDashHeavy");
			add("dotDotDash");
			add("dotDotDashHeavy");
			add("dotted");
			add("dottedHeavy");
			add("heavy");
			add("none");
			add("sng");
			add("wavy");
			add("wavyDbl");
			add("wavyHeavy");
			add("words");
		}

		const std::string TextUnderline::no_find() const
		{
			return "sng";
		}
	} // namespace Limit
} // namespace PPTX