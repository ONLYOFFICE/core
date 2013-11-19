#include "TLPathEditMode.h"

namespace PPTX
{
	namespace Limit
	{
		TLPathEditMode::TLPathEditMode()
		{
			add("fixed");
			add("relative");
		}

		const std::string TLPathEditMode::no_find() const
		{
			return "relative";
		}
	} // namespace Limit
} // namespace PPTX