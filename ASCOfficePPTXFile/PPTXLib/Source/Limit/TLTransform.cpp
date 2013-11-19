#include "TLTransform.h"

namespace PPTX
{
	namespace Limit
	{
		TLTransform::TLTransform()
		{
			add("img");
			add("pt");
		}

		const std::string TLTransform::no_find() const
		{
			return "pt";
		}
	} // namespace Limit
} // namespace PPTX