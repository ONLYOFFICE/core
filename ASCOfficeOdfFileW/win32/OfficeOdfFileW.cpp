// OfficeOdfFileW.cpp : Implementation of COfficeOdfFileW
#include "stdafx.h"

#include "OfficeOdfFileW.h"
#include "../../ASCOfficeUtils/ASCOfficeUtilsLib/OfficeUtils.h"

#include <string>

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp> 

#include "../../Common/DocxFormat/Source/Base/Base.h"
#include "../../Common/DocxFormat/Source/SystemUtility/FileSystem/Directory.h"

#include "../source/Oox2OdfConverter/Oox2OdfConverter.h"

#include <Shlobj.h>

#ifndef STANDALONE_USE
	#define STANDALONE_USE 0// что на входе: файл (1) или папка (0)
#endif 


// им€ директории - uuid

std::wstring bstr2wstring(BSTR str)
{
    return str ? std::wstring(&str[0], &str[::SysStringLen(str)]) : L"";
}

std::wstring COfficeOdfFileW::GetDefWinFontDirectory()
{
	std::wstring strPath;

	wchar_t wsWinFontDir[1024] ={};
	if ( !SHGetSpecialFolderPathW( NULL, wsWinFontDir, CSIDL_FONTS, FALSE ) )
		wsWinFontDir[0] = '\0';


	strPath = std::wstring(wsWinFontDir);
	return strPath;
}
STDMETHODIMP COfficeOdfFileW::LoadFromFile(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions)
{
   return S_FALSE;
}
STDMETHODIMP COfficeOdfFileW::SaveToFile(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions)
{
    HRESULT hr;

    if (!sSrcPath)
    {
        return E_FAIL;
    }

#if defined(STANDALONE_USE) && (STANDALONE_USE == 1)
	std::wstring inputDir = FileSystem::Directory::GetFolderPath(std::wstring(sSrcPath));
#else
	std::wstring inputDir = sSrcPath;
#endif

	std::wstring outputDir = FileSystem::Directory::GetFolderPath(std::wstring(sDstFileName));

    // создаем еЄ в директории куда запишем результат

	std::wstring dstTempPath = FileSystem::Directory::CreateDirectoryWithUniqueName(outputDir);
    

#if defined(STANDALONE_USE) && (STANDALONE_USE == 1)
    std::wstring srcTempPath = FileSystem::Directory::CreateDirectoryWithUniqueName(outputDir);
#else
    std::wstring srcTempPath = inputDir;
#endif

    try
    {
		FileSystem::Directory::CreateDirectory(dstTempPath);

#if defined(STANDALONE_USE) && (STANDALONE_USE == 1)
        
        FileSystem::Directory::CreateDirectory(srcTempPath); // создаем временную директорию дл€ результирующих файлов
#endif
        hr = SaveToFileImpl(bstr2wstring(sSrcPath),srcTempPath, dstTempPath, defaultWinFontPath, bstr2wstring(sDstFileName));
        
    }
    catch(...)
    {
        hr = E_FAIL;
    }

    // стираем временную директорию с распакованным исходником
    try
    {
		FileSystem::Directory::DeleteDirectory(dstTempPath);
    }
    catch(...)
    {
    }

#if defined(STANDALONE_USE) && (STANDALONE_USE == 1)
    // в случае если на выходе файл Ч стираем временную директорию (мы сами ее создали)
    try 
    {
		FileSystem::Directory::DeleteDirectory(srcTempPath);
    }
    catch(...) 
    {
    }
#endif
    
    return hr;
}

HRESULT COfficeOdfFileW::SaveToFileImpl(const std::wstring & srcPath,
                                        const std::wstring & srcTempPath,
                                        const std::wstring & dstTempPath,
										const std::wstring & fontPath,
                                        const std::wstring & dstFileName)
{
    HRESULT hr = E_FAIL;
	COfficeUtils oCOfficeUtils(NULL);  

    // распаковываем исходник (если он файл) во временную директорию
#if defined(STANDALONE_USE) && (STANDALONE_USE == 1)
        if (S_OK != oCOfficeUtils.ExtractToDirectory(srcPath.c_str(), srcTempPath.c_str(), NULL, 0))
            return hr;
#endif

	try
	{
		std::wstring type = DetectTypeDocument(srcTempPath);

		ProgressCallback ffCallBack;

		ffCallBack.OnProgress	=	OnProgressFunc;
		ffCallBack.OnProgressEx	=	OnProgressExFunc;
		ffCallBack.caller		=	this;
		
		Oox2Odf::Converter converter(srcTempPath, type, fontPath, &ffCallBack);
		

		converter.convert();
		converter.write(dstTempPath);
	} 
	catch(...)
	{
	}

	if (S_OK != oCOfficeUtils.CompressFileOrDirectory(dstTempPath.c_str(), dstFileName.c_str(), -1))
        return hr;


    return S_OK;
}

//FileChecker в другом месте !!
std::wstring COfficeOdfFileW::DetectTypeDocument(const std::wstring & Path)
{
	return  L"text";
	//return  L"spreadsheet";
}

void COfficeOdfFileW::OnProgressFunc (LPVOID lpParam, long nID, long nPercent)
{
	COfficeOdfFileW* pOdfFile = reinterpret_cast<COfficeOdfFileW*>(lpParam);
	if (pOdfFile != NULL)
	{
		pOdfFile->OnProgress(nID, nPercent);
	}
}

void COfficeOdfFileW::OnProgressExFunc (LPVOID lpParam, long nID, long nPercent, short* pStop)
{
	COfficeOdfFileW* pOdfFile = reinterpret_cast<COfficeOdfFileW*>(lpParam);
	if (pOdfFile != NULL)
	{
		pOdfFile->OnProgressEx(nID, nPercent, pStop);
	}

}