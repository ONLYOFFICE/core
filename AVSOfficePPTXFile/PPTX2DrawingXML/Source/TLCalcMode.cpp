#include "stdafx.h"
#include "./../Converter.h"

namespace PPTX2DrawingXML
{
	DWORD Converter::GetTLCalcModeFromStr(const std::string& str)
	{
		if(str == "discrete") return 0;
		if(str == "lin") return 1;
		if(str == "fmla") return 2;
		return 0;
	}
}