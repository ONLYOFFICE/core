#include "ColorSchemeIndex.h"

namespace PPTX
{
	namespace Limit
	{
		ColorSchemeIndex::ColorSchemeIndex()
		{
			add("accent1");
			add("accent2");
			add("accent3");
			add("accent4");
			add("accent5");
			add("accent6");
			add("dk1");
			add("dk2");
			add("folHlink");
			add("hlink");
			add("lt1");
			add("lt2");
		}

		const std::string ColorSchemeIndex::no_find() const
		{
			return "accent1";
		}
	} // namespace Limit
} // namespace PPTX