#include "stdafx.h"
#include "./../Converter.h"

namespace PPTX2DrawingXML
{
	WORD Converter::GetTextAlignFromStr(const std::string& str)
	{
		if(str == "l") return 0;
		if(str == "ctr") return 1;
		if(str == "r") return 2;
		if(str == "just") return 3;
		if(str == "dist") return 4;
		if(str == "thaiDist") return 5;
		if(str == "justLow") return 6;
		return 0;
	}
}