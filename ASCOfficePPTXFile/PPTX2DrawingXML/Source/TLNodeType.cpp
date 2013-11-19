#include "stdafx.h"
#include "./../Converter.h"

namespace PPTX2DrawingXML
{
	DWORD Converter::GetTLNodeTypeFromStr(const std::string& str)
	{
		if(str == "clickEffect") return 1;
		if(str == "withEffect") return 2;
		if(str == "afterEffect") return 3;
		if(str == "mainSeq") return 4;
		if(str == "interactiveSeq") return 5;
		if(str == "clickPar") return 6;
		if(str == "withGroup") return 7;
		if(str == "afterGroup") return 8;
		if(str == "tmRoot") return 9;
		return 9;
	}
}