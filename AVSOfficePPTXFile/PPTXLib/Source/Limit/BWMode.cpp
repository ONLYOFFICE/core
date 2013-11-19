#include "BWMode.h"

namespace PPTX
{
	namespace Limit
	{
		BWMode::BWMode()
		{
			add("auto");
			add("black");
			add("blackGray");
			add("blackWhite");
			add("clr");
			add("gray");
			add("grayWhite");
			add("hidden");
			add("invGray");
			add("ltGray");
			add("white");
		}

		const std::string BWMode::no_find() const
		{
			return "auto";
		}
	} // namespace Limit
} // namespace PPTX