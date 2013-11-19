#include "stdafx.h"
#include "./../Converter.h"

namespace PPTX2DrawingXML
{
	DWORD Converter::GetTLPresetClassFromStr(const std::string& str)
	{
		if(str == "entr") return 1;
		if(str == "exit") return 2;
		if(str == "emph") return 3;
		if(str == "path") return 4;
		if(str == "verb") return 5;
		if(str == "mediacall") return 6;
		return 1;
	}
}