// DocFormatTest.cpp : Defines the entry point for the console application.
//
#include "../../../../ASCOfficeDocFile/DocFormatLib/DocFormatLib.h"

#include "../../../../ASCOfficeDocFile/win32/ASCOfficeCriticalSection.h"

#include <string>
#include <tchar.h>

int _tmain(int argc, _TCHAR* argv[])
{
	std::wstring sSrcDoc		= _T("d:\\test\\_doc\\PZ.doc");
    std::wstring sResBackDocx	= _T("d:\\test\\_doc\\PZ.doc-my.docx");

	std::wstring sTemp 			= _T("d:\\home/lena/Documents/temp");

	std::wstring sXMLOptions = _T("");

	// doc->docx
	COfficeDocFile docFile;
	docFile.put_TempDirectory(sTemp);
	
	HRESULT hRes = docFile.LoadFromFile( sSrcDoc, sResBackDocx, NULL);
	
	if (hRes != S_OK)return 2;
	

	return 0;
}

