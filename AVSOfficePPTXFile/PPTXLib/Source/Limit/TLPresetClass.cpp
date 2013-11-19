#include "TLPresetClass.h"

namespace PPTX
{
	namespace Limit
	{
		TLPresetClass::TLPresetClass()
		{
			add("emph");
			add("entr");
			add("exit");
			add("mediacall");
			add("path");
			add("verb");
		}

		const std::string TLPresetClass::no_find() const
		{
			return "exit";
		}
	} // namespace Limit
} // namespace PPTX