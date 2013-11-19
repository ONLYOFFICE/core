#include "TLTriggerEvent.h"

namespace PPTX
{
	namespace Limit
	{
		TLTriggerEvent::TLTriggerEvent()
		{
			add("begin");
			add("end");
			add("onBegin");
			add("onClick");
			add("onDblClick");
			add("onEnd");
			add("onMouseOut");
			add("onMouseOver");
			add("onNext");
			add("onPrev");
			add("onStopAudio");
		}

		const std::string TLTriggerEvent::no_find() const
		{
			return "begin";
		}
	} // namespace Limit
} // namespace PPTX