#include "TLRestart.h"

namespace PPTX
{
	namespace Limit
	{
		TLRestart::TLRestart()
		{
			add("always");
			add("never");
			add("whenNotActive");
		}

		const std::string TLRestart::no_find() const
		{
			return "never";
		}
	} // namespace Limit
} // namespace PPTX