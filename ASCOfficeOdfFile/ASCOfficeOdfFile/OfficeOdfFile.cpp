/// \file   OfficeOdfFile.cpp
#include "stdafx.h"

#include "OfficeOdfFile.h"
#include "../../ASCOfficeUtils/ASCOfficeUtilsLib/OfficeUtils.h"

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/lexical_cast.hpp>

#include "../../Common/DocxFormat/Source/Base/Base.h"
#include "../../Common/DocxFormat/Source/SystemUtility/FileSystem/Directory.h"

#include "ConvertOO2OOX.h"

#include "../../Common/XmlUtils.h"

#include "../../Common/ASCATLError.h"

#include <string>

// ВНИМАНИЕ:    значение 1 используется для тестирования, на выходе получаем заархивированный файл xlsx или docx
//              значение 0 используется для релиза, так как на выходе по спецификации нам требуется распакованный package
#ifndef STANDALONE_USE
	#define STANDALONE_USE 0// что получаем на выходе: файл (1) или папку (0)
#endif


COfficeOdfFile::COfficeOdfFile()
{
	bOnlyPresentation = false;
}

namespace {


std::wstring bstr2wstring(BSTR str)
{
    return str ? std::wstring(&str[0], &str[::SysStringLen(str)]) : L"";
}


}

bool COfficeOdfFile::loadOptionFromXML(CString parametr,BSTR sXMLOptions)
{
	bool result = false;
	XmlUtils::CXmlReader oXmlReader;
	if( TRUE == oXmlReader.OpenFromXmlString( sXMLOptions ) )
	{
		if( TRUE == oXmlReader.ReadRootNode( _T("Options") ) )
		{
			if( TRUE == oXmlReader.ReadNode( parametr ) )
			{
				result = true;
			}
		}		
	}

	return result;

}


HRESULT COfficeOdfFile::LoadFromFile(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions)
{
#if defined(STANDALONE_USE) && (STANDALONE_USE == 1)
	std::wstring outputDir = FileSystem::Directory::GetFolderPath(std::wstring(sDstPath));
#else
	std::wstring outputDir = sDstPath;
#endif

    // временная директория, в которую распакуем исходный файл,
    // создаем её в директории куда запишем результат

	std::wstring srcTempPath = FileSystem::Directory::CreateDirectoryWithUniqueName(outputDir);
    
#if defined(STANDALONE_USE) && (STANDALONE_USE == 1)
	std::wstring dstTempPath = FileSystem::Directory::CreateDirectoryWithUniqueName(outputDir);
#else
    std::wstring dstTempPath = outputDir;
#endif

	bOnlyPresentation = loadOptionFromXML(L"onlyPresentation", sXMLOptions);

    HRESULT hr;
	try
    {
        FileSystem::Directory::CreateDirectory(srcTempPath);
#if defined(STANDALONE_USE) && (STANDALONE_USE == 1)
        // создаем временную директорию для результирующих файлов
		FileSystem::Directory::CreateDirectory(dstTempPath);
#endif
        //hr = LoadFromFileImpl(bstr2wstring(sSrcFileName), srcTempPath.string<std::wstring>(), dstTempPath.string<std::wstring>(), bstr2wstring(sDstPath));
        hr = LoadFromFileImpl(bstr2wstring(sSrcFileName), srcTempPath, dstTempPath, bstr2wstring(sDstPath));
        
    }
    catch(...)
    {
        hr = E_FAIL;
    }

    // стираем временную директорию с распакованным исходником
    try
    {
		FileSystem::Directory::DeleteDirectory(srcTempPath);
    }
    catch(...) 
    {
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

HRESULT COfficeOdfFile::LoadFromFileImpl(const std::wstring & srcFileName,
                                         const std::wstring & srcTempPath,
                                         const std::wstring & dstTempPath,
                                         const std::wstring & dstPath)
{
    HRESULT hr = AVS_ERROR_UNEXPECTED;    
        
    // распаковываем исходник во временную директорию
	COfficeUtils oCOfficeUtils(NULL);
    if (S_OK != oCOfficeUtils.ExtractToDirectory(srcFileName.c_str(), srcTempPath.c_str(), NULL, 0))
		return hr;

	ProgressCallback ffCallBack;

	ffCallBack.OnProgress	=	OnProgressFunc;
	ffCallBack.OnProgressEx	=	OnProgressExFunc;
	ffCallBack.caller		=	this;

	hr = ConvertOO2OOX(srcTempPath, dstTempPath,bOnlyPresentation, &ffCallBack);

	if (hr != S_OK)  return hr;
   
#if defined(STANDALONE_USE) && (STANDALONE_USE == 1)
	if (S_OK != oCOfficeUtils.CompressFileOrDirectory(dstTempPath.c_str(), dstPath.c_str(), -1))
        return hr;
#endif

    return S_OK;
}


HRESULT COfficeOdfFile::SaveToFile(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions)
{
    return E_NOTIMPL;
}

void COfficeOdfFile::OnProgressFunc (LPVOID lpParam, long nID, long nPercent)
{
	COfficeOdfFile* pOdfFile = reinterpret_cast<COfficeOdfFile*>(lpParam);
	if (pOdfFile != NULL)
	{
		pOdfFile->OnProgress(nID, nPercent);
	}
}

void COfficeOdfFile::OnProgressExFunc (LPVOID lpParam, long nID, long nPercent, short* pStop)
{
	COfficeOdfFile* pOdfFile = reinterpret_cast<COfficeOdfFile*>(lpParam);
	if (pOdfFile != NULL)
	{
		pOdfFile->OnProgressEx(nID, nPercent, pStop);
	}
}
