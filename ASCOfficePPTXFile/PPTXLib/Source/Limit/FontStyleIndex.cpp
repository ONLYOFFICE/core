#include "FontStyleIndex.h"

namespace PPTX
{
	namespace Limit
	{
		FontStyleIndex::FontStyleIndex()
		{
			add("major");
			add("minor");
			add("none");
		}

		const std::string FontStyleIndex::no_find() const
		{
			return "none";
		}
	} // namespace Limit
} // namespace PPTX