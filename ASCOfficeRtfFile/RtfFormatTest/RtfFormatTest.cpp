// DocFormatTest.cpp : Defines the entry point for the console application.
//
#include <tchar.h>
#include "../RtfFormatLib/source/ConvertationManager.h"

#include <algorithm>
#include <string>

int _tmain(int argc, _TCHAR* argv[])
{
	std::wstring sTemp 			= _T("D:\\test\\Temp");

	std::wstring sArg_1 = argv[1];
	std::wstring sArg_2 = argv[2];

	int n1 = sArg_1.rfind(_T('.'));
	int n2 = sArg_2.rfind(_T('.'));

	std::wstring ext_1 = n1>=0 ? sArg_1.substr(n1+1, sArg_1.length() - n1): _T(""); //ext_1.MakeLower();
	std::wstring ext_2 = n2>=0 ? sArg_2.substr(n2+1, sArg_2.length() - n2): _T(""); //ext_2.MakeLower();

	std::transform(ext_1.begin(), ext_1.end(), ext_1.begin(), ::tolower);

	RtfConvertationManager rtfConvert;

	rtfConvert.m_sTempFolder = sTemp;

	if (ext_1 == _T("rtf"))
	{	// rtf->docx
		rtfConvert.ConvertRtfToOOX(sArg_1, sArg_2);
	}
	if (ext_2 == _T("rtf"))
	{	// docx->rtf
		rtfConvert.ConvertOOXToRtf(sArg_2, sArg_1);
	}
	return 0;
}

