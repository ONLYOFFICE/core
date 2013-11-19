#include "VariantType.h"


namespace PPTX
{
	namespace Limit
	{
		VariantType::VariantType()
		{
			add("lpstr");
			add("i4");
		}

		const std::string VariantType::no_find() const
		{
			return "lpstr";
		}
	} // namespace Limit
} // namespace PPTX