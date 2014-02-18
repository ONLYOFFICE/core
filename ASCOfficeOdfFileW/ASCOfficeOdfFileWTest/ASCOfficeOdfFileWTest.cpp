// ASCOfficeOdfFileWTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <boost/timer.hpp>
#include <iostream>
#include <string>

#import "..\..\Redist\ASCOfficeOdfFileW.dll" rename_namespace("ASCOfficeOdfFileW") raw_interfaces_only

#define HR_RET(HR) if FAILED(hr = (HR)) { _ASSERTE(false); return -1; }

int ConvertSingle(int argc, _TCHAR* argv[])
{
    ATL::CComPtr<ASCOfficeOdfFileW::IAVSOfficeFileTemplate> officeOdfFileW;
    HRESULT hr;
	HR_RET(officeOdfFileW.CoCreateInstance(__uuidof(ASCOfficeOdfFileW::COfficeOdfFileW)));

    boost::timer t1;
	officeOdfFileW->SaveToFile(ATL::CComBSTR(argv[2]), ATL::CComBSTR(argv[1]), NULL);

    std::cout << "\n\nTime : " << t1.elapsed() << "\n";    
    return 0;
}


int _tmain(int argc, _TCHAR* argv[])
{
    CoInitialize(NULL);
    if (argc < 3)
        return -1;

    return ConvertSingle(argc, argv);
	return 0;
}
