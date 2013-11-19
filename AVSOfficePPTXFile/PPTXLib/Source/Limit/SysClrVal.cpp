#include "SysClrVal.h"

namespace PPTX
{
	namespace Limit
	{
		SysClrVal::SysClrVal()
		{
			add("3dDkShadow");
			add("3dLight");
			add("activeBorder");
			add("activeCaption");
			add("appWorkspace");
			add("background");
			add("btnFace");
			add("btnHighlight");
			add("btnShadow");
			add("btnText");
			add("captionText");
			add("gradientActiveCaption");
			add("gradientInactiveCaption");
			add("grayText");
			add("highlight");
			add("highlightText");
			add("hotLight");
			add("inactiveBorder");
			add("inactiveCaption");
			add("inactiveCaptionText");
			add("infoBk");
			add("infoText");
			add("menu");
			add("menuBar");
			add("menuHighlight");
			add("menuText");
			add("scrollBar");
			add("window");
			add("windowFrame");
			add("windowText");
 		}

		const std::string SysClrVal::no_find() const
		{
			return "window";
		}
	} // namespace Limit
} // namespace PPTX