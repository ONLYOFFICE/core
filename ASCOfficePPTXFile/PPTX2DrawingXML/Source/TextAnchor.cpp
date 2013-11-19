#include "stdafx.h"
#include "./../Converter.h"

namespace PPTX2DrawingXML
{
	WORD Converter::GetTextAnchorFromStr(const std::string& str)
	{
		if(str == "t") return 0;
		if(str == "ctr") return 1;
		if(str == "b") return 2;
		return 0;
	}
}