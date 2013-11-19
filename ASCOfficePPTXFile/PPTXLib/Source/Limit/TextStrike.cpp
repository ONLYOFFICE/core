#include "TextStrike.h"

namespace PPTX
{
	namespace Limit
	{
		TextStrike::TextStrike()
		{
			add("dblStrike");
			add("noStrike");
			add("sngStrike");
		}

		const std::string TextStrike::no_find() const
		{
			return "noStrike";
		}
	} // namespace Limit
} // namespace PPTX