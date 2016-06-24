/*
 * (c) Copyright Ascensio System SIA 2010-2016
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
// OfficeOdfFileW.cpp : Implementation of COfficeOdfFileW
#include "stdafx.h"

#include "OfficeOdfFileW.h"
#include "../../OfficeUtils/src/OfficeUtils.h"

#include <string>

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp> 

#include "../../Common/DocxFormat/Source/Base/Base.h"
#include "../../Common/DocxFormat/Source/SystemUtility/FileSystem/Directory.h"
#include "../../Common/DocxFormat/Source/SystemUtility/File.h"

#include "../source/Oox2OdfConverter/Oox2OdfConverter.h"

#include <Shlobj.h>

#ifndef STANDALONE_USE
	#define STANDALONE_USE 0// что на входе: файл (1) или папка (0)
#endif 


// имя директории - uuid

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

    // создаем её в директории куда запишем результат

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
        
        FileSystem::Directory::CreateDirectory(srcTempPath); // создаем временную директорию для результирующих файлов
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

std::wstring COfficeOdfFileW::DetectTypeDocument(const std::wstring & pathOOX)
{
	std::wstring sRes;

	CFile file;

	CString fileContentType = std_string2string(pathOOX + FILE_SEPARATOR_STR + L"[Content_Types].xml");

	if (file.OpenFile(fileContentType) != S_OK) return sRes;

	int nBufferSize = min (file.GetFileSize(), 4096);
	BYTE *pBuffer = new BYTE[nBufferSize];

	file.ReadFile(pBuffer, nBufferSize);
	file.CloseFile();

	if (pBuffer != NULL)
	{

		const char *docxFormatLine = "application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml";
		const char *dotxFormatLine = "application/vnd.openxmlformats-officedocument.wordprocessingml.template.main+xml";
		const char *docmFormatLine = "application/vnd.ms-word.document.macroEnabled.main+xml";
		const char *dotmFormatLine = "application/vnd.ms-word.template.macroEnabledTemplate.main+xml";

		const char *xlsxFormatLine = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet.main+xml";
		const char *xltxFormatLine = "application/vnd.openxmlformats-officedocument.spreadsheetml.template.main+xml";
		const char *xlsmFormatLine = "application/vnd.ms-excel.sheet.macroEnabled.main+xml";
		const char *xltmFormatLine = "application/vnd.ms-excel.template.macroEnabled.main+xml";

		const char *pptxFormatLine = "application/vnd.openxmlformats-officedocument.presentationml.presentation.main+xml";
		const char *ppsxFormatLine = "application/vnd.openxmlformats-officedocument.presentationml.slideshow.main+xml";
		const char *potxFormatLine = "application/vnd.openxmlformats-officedocument.presentationml.template.main+xml";
		const char *pptmFormatLine = "application/vnd.ms-powerpoint.presentation.macroEnabled.main+xml";
		const char *ppsmFormatLine = "application/vnd.ms-powerpoint.slideshow.macroEnabled.main+xml";
		const char *potmFormatLine = "application/vnd.ms-powerpoint.template.macroEnabled.main+xml";

        std::string strContentTypes((char*)pBuffer, nBufferSize);

        int res = 0;
        if ( (res = strContentTypes.find(docxFormatLine))>0 || (res = strContentTypes.find(dotxFormatLine))>0 ||
            (res = strContentTypes.find(docmFormatLine))>0 || (res = strContentTypes.find(dotmFormatLine))>0)
		{
			sRes = L"text";
		}

        else if ((res = strContentTypes.find(xlsxFormatLine))>0 || (res = strContentTypes.find(xltxFormatLine))>0 ||
            (res = strContentTypes.find(xlsmFormatLine))>0 || (res = strContentTypes.find(xltmFormatLine))>0)
		{
			sRes = L"spreadsheet";
		}

        else if ((res = strContentTypes.find(pptxFormatLine) > 0) || /*(res = strContentTypes.find(ppsxFormatLine))>0 ||*/
            (res = strContentTypes.find(potxFormatLine))>0 || (res = strContentTypes.find(pptmFormatLine))>0 ||
            (res = strContentTypes.find(ppsmFormatLine))>0 || (res = strContentTypes.find(potmFormatLine))>0 ||
			(res = strContentTypes.find(ppsxFormatLine)) >0 )
		{
		}

		delete []pBuffer;
		pBuffer = NULL;

	}
	return sRes;
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