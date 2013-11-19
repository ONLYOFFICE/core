#include "stdafx.h"
#include "./../Converter.h"

namespace Odp2DrawingXML
{
	WORD Converter::GetTextAnchorFromStr(const std::string& str)
	{
		if(str == "top") return 0;
		if((str == "center") || (str == "middle"))return 1;
		if(str == "bottom") return 2;
		return 0;
	}
}