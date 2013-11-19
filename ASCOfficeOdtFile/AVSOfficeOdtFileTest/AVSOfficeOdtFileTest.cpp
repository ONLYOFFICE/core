// AVSOfficeOdfFileTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <boost/timer.hpp>
#include <iostream>
#include <string>

#import "libid:E2A541D8-4CC7-40C6-AA18-A033006D8E5A" rename_namespace("AVSOfficeOdtFile"), raw_interfaces_only

#define HR_RET(HR) if FAILED(hr = (HR)) { _ASSERTE(false); return -1; }

int ConvertSingle(int argc, _TCHAR* argv[])
{
    ATL::CComPtr<AVSOfficeOdtFile::IAVSOfficeFileTemplate> officeOdtFile;
    HRESULT hr;
    HR_RET(officeOdtFile.CoCreateInstance(__uuidof(AVSOfficeOdtFile::COdtFile)));

    boost::timer t1;
	officeOdtFile->SaveToFile(ATL::CComBSTR(argv[1]), ATL::CComBSTR(argv[2]), NULL);

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
