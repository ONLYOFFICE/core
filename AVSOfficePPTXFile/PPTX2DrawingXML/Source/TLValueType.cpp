#include "stdafx.h"
#include "./../Converter.h"

namespace PPTX2DrawingXML
{
	DWORD Converter::GetTLValueTypeFromStr(const std::string& str)
	{
		if(str == "str") return 0;
		if(str == "num") return 1;
		if(str == "clr") return 2;
		return 1;
	}
}