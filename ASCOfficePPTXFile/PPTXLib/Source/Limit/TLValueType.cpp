#include "TLValueType.h"

namespace PPTX
{
	namespace Limit
	{
		TLValueType::TLValueType()
		{
			add("num");
			add("clr");
			add("str");
		}

		const std::string TLValueType::no_find() const
		{
			return "num";
		}
	} // namespace Limit
} // namespace PPTX