#include "SlideSize.h"

namespace PPTX
{
	namespace Limit
	{
		SlideSize::SlideSize()
		{
			add("35mm");
			add("A3");
			add("A4");
			add("B4ISO");
			add("B4JIS");
			add("B5ISO");
			add("B5JIS");
			add("banner");
			add("custom");
			add("hagakiCard");
			add("ledger");
			add("letter");
			add("overhead");
			add("screen16x10");
			add("screen16x9");
			add("screen4x3");
		}

		const std::string SlideSize::no_find() const
		{
			return "screen4x3";
		}
	} // namespace Limit
} // namespace PPTX