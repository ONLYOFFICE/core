// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../../../include/xmlutils.h"

#define ASC_VS

#ifdef _DEBUG
#ifdef ASC_VS
#pragma comment (lib, "../../vs2013/x64/Debug/libxml.lib")
#else
#pragma comment (lib, "../../qt/Debug/debug/libxml.lib")
#endif
#else
#ifdef ASC_VS
#pragma comment (lib, "../../vs2013/x64/Release/libxml.lib")
#else
#pragma comment (lib, "../../qt/Release/release/libxml.lib")
#endif
#endif


int _tmain(int argc, _TCHAR* argv[])
{
	XmlUtils::CXmlNode oNode;
	oNode.FromXmlFile(L"D:\\document.xml");
	return 0;
}

