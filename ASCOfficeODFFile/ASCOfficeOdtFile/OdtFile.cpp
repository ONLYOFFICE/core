// OdtFile.cpp : Implementation of COdtFile

#include "stdafx.h"
#include "OdtFile.h"

#import "../../Redist/ASCOfficeOdfFile.dll" rename_namespace("ASCOfficeOdfFile"), raw_interfaces_only

COdtFile::COdtFile()
{   
}

COdtFile::~COdtFile()
{
}

HRESULT COdtFile::FinalConstruct()
{
    HRESULT hr = odfFile_.CoCreateInstance(__uuidof(ASCOfficeOdfFile::COfficeOdfFile));
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
