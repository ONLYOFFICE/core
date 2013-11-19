#include "SchemeClrVal.h"

namespace PPTX
{
	namespace Limit
	{
		SchemeClrVal::SchemeClrVal()
		{
			add("accent1"); // (Accent Color 1)  Extra scheme color 1 
			add("accent2"); // (Accent Color 2)  Extra scheme color 2 
			add("accent3"); // (Accent Color 3)  Extra scheme color 3 
			add("accent4"); // (Accent Color 4)  Extra scheme color 4 
			add("accent5"); // (Accent Color 5)  Extra scheme color 5 
			add("accent6"); // (Accent Color 6)  Extra scheme color 6 
			add("bg1"); // (Background Color 1)  Semantic background color 
			add("bg2"); // (Background Color 2)  Semantic additional background color 
			add("dk1"); // (Dark Color 1)  Main dark color 1 
			add("dk2"); // (Dark Color 2)  Main dark color 2 
			add("folHlink"); // (Followed Hyperlink Color)  Followed Hyperlink Color 
			add("hlink"); // (Hyperlink Color)  Regular Hyperlink Color 
			add("lt1"); // (Light Color 1)  Main Light Color 1 
			add("lt2"); // (Light Color 2)  Main Light Color 2 
			add("phClr"); // (Style Color)  A color used in theme definitions which means to use 
			add("tx1"); // (Text Color 1)  Semantic text color 
			add("tx2"); // (Text Color 2)  Semantic additional text color 
 		}

		const std::string SchemeClrVal::no_find() const
		{
			return "accent1";
		}
	} // namespace Limit
} // namespace PPTX