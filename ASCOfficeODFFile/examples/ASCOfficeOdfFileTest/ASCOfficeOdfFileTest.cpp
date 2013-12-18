// ASCOfficeOdfFileTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <boost/timer.hpp>
#include <iostream>
#include <string>

#import "../../../Redist/ASCOfficeOdfFile.dll" rename_namespace("ASCOfficeOdfFile"), raw_interfaces_only
#import "../../../Redist/ASCPresentationEditor.dll" rename_namespace("Editor"), raw_interfaces_only

#define HR_RET(HR) if FAILED(hr = (HR)) { _ASSERTE(false); return -1; }

int ConvertSingle(int argc, _TCHAR* argv[])
{
    ATL::CComPtr<ASCOfficeOdfFile::IASCOfficeFileTemplate> officeOdfFile;
    HRESULT hr;
    HR_RET(officeOdfFile.CoCreateInstance(__uuidof(ASCOfficeOdfFile::COfficeOdfFile)));

    boost::timer t1;
    officeOdfFile->LoadFromFile(ATL::CComBSTR(argv[1]), ATL::CComBSTR(argv[2]), NULL);
    std::cout << "\n\nTime : " << t1.elapsed() << "\n";    
    return 0;
}

int PresentationEditor(int argc, _TCHAR* argv[])
{
	ATL::CComPtr<Editor::IAVSPresentationEditor> officeEditor;
    HRESULT hr;
	HR_RET(officeEditor.CoCreateInstance(__uuidof(Editor::CAVSPresentationEditor)));

    boost::timer t1;
	hr = officeEditor->ConvertToPPTY(ATL::CComBSTR(argv[1]),ATL::CComBSTR(argv[2]));
   //hr = officeEditor->OpenFile(ATL::CComBSTR(argv[1]));
    std::cout << "\n\nTime : " << t1.elapsed() << "\n";    
    return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
    CoInitialize(NULL);
   
	//return PresentationEditor(argc, argv);
	return ConvertSingle(argc, argv);


	return 0;
}
