// ASCOfficeOdfFileTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <boost/timer.hpp>
#include <iostream>
#include <string>

#import "../../../Redist/ASCOfficeOdfFile.dll" rename_namespace("ASCOfficeOdfFile"), raw_interfaces_only

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

int ConvertMulti(int argc, _TCHAR* argv[])
{
    ATL::CComPtr<ASCOfficeOdfFile::IASCOfficeFileTemplate> officeOdfFile;
    HRESULT hr;
    HR_RET(officeOdfFile.CoCreateInstance(__uuidof(ASCOfficeOdfFile::COfficeOdfFile)));

    boost::timer t1;

    for (int i = 0; i < argc - 2; ++i)
    {
        const std::wstring input = argv[i + 2];
        const std::wstring output = input + std::wstring(L".docx");
        officeOdfFile->LoadFromFile(ATL::CComBSTR(input.c_str()), ATL::CComBSTR(output.c_str()), NULL);
    }

    std::cout << "\n\nTime : " << t1.elapsed() << "\n";
    return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
    CoInitialize(NULL);
    if (argc < 3)
        return -1;

    if (std::wstring(argv[1]) == L"-m")
    {
        return ConvertMulti(argc, argv);            
    }
    else
    {
        return ConvertSingle(argc, argv);
    }

	return 0;
}
