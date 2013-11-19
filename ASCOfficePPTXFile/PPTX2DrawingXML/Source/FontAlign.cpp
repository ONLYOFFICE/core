#include "stdafx.h"
#include "./../Converter.h"

namespace PPTX2DrawingXML
{
	WORD Converter::GetFontAlignFromStr(const std::string& str)
	{
		if(str == "auto") return 0;
		if(str == "base") return 0;
		if(str == "t") return 1;
		if(str == "ctr") return 2;
		if(str == "b") return 3;
		return 0;
	}
}