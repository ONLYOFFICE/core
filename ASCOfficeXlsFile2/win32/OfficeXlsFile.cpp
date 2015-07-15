// OfficeXlsFile.cpp : Implementation of COfficeXlsFile

#include "stdafx.h"

#include <string>
#include <iostream>

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/lexical_cast.hpp>

#include "../../Common/DocxFormat/Source/Base/Base.h"
#include "../../Common/DocxFormat/Source/SystemUtility/FileSystem/Directory.h"

#include "../source/XlsXlsxConverter/ConvertXls2Xlsx.h"
#include "../source/XlsXlsxConverter/progressCallback.h"

#include "../Common/XmlUtils.h"

#include "../../ASCOfficeUtils/ASCOfficeUtilsLib/OfficeUtils.h"
#include "../../Common/ASCATLError.h"

#include "OfficeXlsFile.h"

// ВНИМАНИЕ:    значение 1 используется для тестирования, на выходе получаем заархивированный файл xlsx или docx
//              значение 0 используется для релиза, так как на выходе по спецификации нам требуется распакованный package
#ifndef STANDALONE_USE
	#define STANDALONE_USE 0// что получаем на выходе: файл (1) или папку (0)
#endif



std::wstring bstr2wstring(BSTR str)
{
    return str ? std::wstring(&str[0], &str[::SysStringLen(str)]) : L"";
}

///------------------------------------------------------------------------------------

// COfficeXlsFile
COfficeXlsFile::COfficeXlsFile()
{
}

HRESULT COfficeXlsFile::SaveToFile(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions)
{
    return E_NOTIMPL;
}

HRESULT COfficeXlsFile::LoadFromFile(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions)
{
 	HRESULT hr = AVS_ERROR_UNEXPECTED;  

	if (!sDstPath)
    {
        _ASSERTE(!!sDstPath);
        return E_FAIL;
    }

#if defined(STANDALONE_USE) && (STANDALONE_USE == 1)
	std::wstring outputDir = FileSystem::Directory::GetFolderPath(std::wstring(sDstPath));
#else
	std::wstring outputDir = sDstPath;
#endif

#if defined(STANDALONE_USE) && (STANDALONE_USE == 1)
	std::wstring dstTempPath = FileSystem::Directory::CreateDirectoryWithUniqueName(outputDir);
#else
    std::wstring dstTempPath = outputDir;
#endif

    try
    {

        hr = LoadFromFileImpl(bstr2wstring(sSrcFileName), dstTempPath, bstr2wstring(sDstPath));
        
    }
    catch(...)
    {
        hr = E_FAIL;
    }



#if defined(STANDALONE_USE) && (STANDALONE_USE == 1)
    // в случае если на выходе файл — стираем временную директорию (мы сами ее создали)
    try 
    {
		FileSystem::Directory::DeleteDirectory(dstTempPath);
    }
    catch(...)
    {
    }
#endif
     
    return hr;
}



HRESULT COfficeXlsFile::LoadFromFileImpl(const std::wstring & srcFileName, const std::wstring & dstTempPath, const std::wstring & dstPath)
{
	HRESULT hr = AVS_ERROR_UNEXPECTED;  

	ProgressCallback ffCallBack;

	ffCallBack.OnProgress	=	OnProgressFunc;
	ffCallBack.OnProgressEx	=	OnProgressExFunc;
	ffCallBack.caller		=	this;

	hr = ConvertXls2Xlsx(srcFileName, dstTempPath, &ffCallBack);

	if (hr != S_OK)  return hr;
   
#if defined(STANDALONE_USE) && (STANDALONE_USE == 1)
	COfficeUtils oCOfficeUtils(NULL);
	if (S_OK != oCOfficeUtils.CompressFileOrDirectory(dstTempPath.c_str(), dstPath.c_str(), -1))
        return hr;
#endif

    return S_OK;
}

void COfficeXlsFile::OnProgressFunc (LPVOID lpParam, long nID, long nPercent)
{
	COfficeXlsFile* pXlsFile = reinterpret_cast<COfficeXlsFile*>(lpParam);
	if (pXlsFile != NULL)
	{
		pXlsFile->OnProgress(nID, nPercent);
	}
}

void COfficeXlsFile::OnProgressExFunc (LPVOID lpParam, long nID, long nPercent, short* pStop)
{
	COfficeXlsFile* pXlsFile = reinterpret_cast<COfficeXlsFile*>(lpParam);
	if (pXlsFile != NULL)
	{
		pXlsFile->OnProgressEx(nID, nPercent, pStop);
	}
}
