// ASCOfficeWSHtmlFileTest.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

#ifdef _DEBUG
#import "..\ASCOfficeWSHtmlFile\bin\Debug\ASCOfficeWSHtmlFile.dll"							rename_namespace("AVSOfficeWSHtmlFile")
#else
#import "..\ASCOfficeWSHtmlFile\bin\Release\ASCOfficeWSHtmlFile.dll"						rename_namespace("AVSOfficeWSHtmlFile")
#endif

#include <string>

int _tmain(int argc, _TCHAR* argv[])
{
		CoInitialize(NULL);

		const std::wstring inputPath  = L"c:\\OriginXlsx";
		const std::wstring outputPath = L"c:\\HtmlFromXlsx.html";

		AVSOfficeWSHtmlFile::IWSHtmlFilePtr ptr_html(__uuidof( AVSOfficeWSHtmlFile::CWSHtmlFile));
		ptr_html->SaveToFile(outputPath.c_str(), inputPath.c_str(), L"");
		ptr_html.Release();

		CoUninitialize();
		return 0;
}

