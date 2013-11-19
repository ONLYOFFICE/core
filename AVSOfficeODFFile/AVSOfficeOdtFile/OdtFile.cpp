// OdtFile.cpp : Implementation of COdtFile

#include "stdafx.h"
#include "OdtFile.h"

#import "libid:9AEF1F19-91CE-46C0-A380-06A7F56CE4AE" rename_namespace("AVSOfficeOdfFile"), raw_interfaces_only

COdtFile::COdtFile()
{   
}

COdtFile::~COdtFile()
{
}

HRESULT COdtFile::FinalConstruct()
{
    HRESULT hr = odfFile_.CoCreateInstance(__uuidof(AVSOfficeOdfFile::COfficeOdfFile));
    return hr;
}

void COdtFile::FinalRelease()
{
}


HRESULT COdtFile::LoadFromFile(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions)
{
    SHORT val;
    OnProgressEx(0, 0, &val);
    HRESULT hr = odfFile_->LoadFromFile(sSrcFileName, sDstPath, sXMLOptions);
    OnProgressEx(0, 1000000, &val);
    return hr;
}

HRESULT COdtFile::SaveToFile(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions)
{
    return odfFile_->SaveToFile(sDstFileName, sSrcPath, sXMLOptions);
}

// COdtFile
