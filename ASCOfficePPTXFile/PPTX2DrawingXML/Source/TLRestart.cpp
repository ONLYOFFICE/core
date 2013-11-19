#include "stdafx.h"
#include "./../Converter.h"

namespace PPTX2DrawingXML
{
	DWORD Converter::GetTLRestartFromStr(const std::string& str)
	{
		if(str == "never") return 0;
		if(str == "always") return 1;
		if(str == "whenNotActive") return 2;
		return 0;
	}
}