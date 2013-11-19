#include "stdafx.h"
#include "./../Converter.h"

namespace Odp2DrawingXML
{
	WORD Converter::GetTextAlignFromStr(const std::string& str)
	{
		if((str == "left") || (str == "begin"))return 0;
		if(str == "center") return 1;
		if(str == "end") return 2;
		if(str == "justify") return 3;
//		if(str == "dist") return 4;
//		if(str == "thaiDist") return 5;
//		if(str == "justLow") return 6;
		return 0;
	}
}