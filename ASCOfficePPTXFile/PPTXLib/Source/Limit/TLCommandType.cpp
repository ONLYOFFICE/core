#include "TLCommandType.h"

namespace PPTX
{
	namespace Limit
	{
		TLCommandType::TLCommandType()
		{
			add("call");
			add("evt");
			add("verb");			
		}

		const std::string TLCommandType::no_find() const
		{
			return "verb";
		}
	} // namespace Limit
} // namespace PPTX