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
/// \file   OfficeOdfFile.cpp
#include "stdafx.h"

#include "OfficeOdfFile.h"
#include "../../ASCOfficeUtils/ASCOfficeUtilsLib/OfficeUtils.h"

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/lexical_cast.hpp>

#include "../../Common/DocxFormat/Source/Base/Base.h"
#include "../../Common/DocxFormat/Source/SystemUtility/FileSystem/Directory.h"

#include "../src/ConvertOO2OOX.h"

#include "../../Common/XmlUtils.h"

#include "../../Common/ASCATLError.h"

#include <string>
#include <Shlobj.h>


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
		std::wstring fontsPath = GetDefWinFontDirectory();
///

///
        hr = LoadFromFileImpl(bstr2wstring(sSrcFileName), srcTempPath, dstTempPath, bstr2wstring(sDstPath), fontsPath);
        
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
                                         const std::wstring & dstPath,
										 const std::wstring & fontsPath)
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

	hr = ConvertOO2OOX(srcTempPath, dstTempPath, fontsPath, bOnlyPresentation, &ffCallBack);

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
std::wstring COfficeOdfFile::GetDefWinFontDirectory()
{
	std::wstring strPath;

	wchar_t wsWinFontDir[1024] ={};
	if ( !SHGetSpecialFolderPathW( NULL, wsWinFontDir, CSIDL_FONTS, FALSE ) )
		wsWinFontDir[0] = '\0';


	strPath = std::wstring(wsWinFontDir);
	return strPath;
}
