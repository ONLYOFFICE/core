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
// ASCOfficeOdfFileWTest.cpp : Defines the entry point for the console application.
//

#include "../../OfficeUtils/src/OfficeUtils.h"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp> 
#include <boost/timer.hpp>

#include <iostream>
#include <string>

#include "../../Common/DocxFormat/Source/Base/Base.h"
#include "../../Common/DocxFormat/Source/SystemUtility/FileSystem/Directory.h"
#include "../../Common/DocxFormat/Source/SystemUtility/File.h"

#include "../source/Oox2OdfConverter/Oox2OdfConverter.h"

#if defined(_WIN64)
	#pragma comment(lib, "../../build/bin/icu/win_64/icuuc.lib")
#elif defined (_WIN32)
	#pragma comment(lib, "../../build/bin/icu/win_32/icuuc.lib")
#endif

std::wstring DetectTypeDocument(const std::wstring & pathOOX)
{
	std::wstring sRes;

	CFile file;

	CString fileContentType = std_string2string(pathOOX + FILE_SEPARATOR_STR + L"[Content_Types].xml");

	if (file.OpenFile(fileContentType) != S_OK) return sRes;

	int nBufferSize = min (file.GetFileSize(), 10000);
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


int _tmain(int argc, _TCHAR* argv[])
{
	if (argc < 3) return 0;

	HRESULT hr = S_OK;
    boost::timer t1;
//////////////////////////////////////////////////////////////////////////
	std::wstring srcFileName	= argv[1];
	std::wstring dstPath		= argv[2];
	std::wstring outputDir		= FileSystem::Directory::GetFolderPath(dstPath);
	
	std::wstring srcTempPath	= FileSystem::Directory::CreateDirectoryWithUniqueName(outputDir);
	std::wstring dstTempPath	= FileSystem::Directory::CreateDirectoryWithUniqueName(outputDir);

    // распаковываем исходник во временную директорию
	COfficeUtils oCOfficeUtils(NULL);
    if (S_OK != oCOfficeUtils.ExtractToDirectory(srcFileName.c_str(), srcTempPath.c_str(), NULL, 0))
		return S_FALSE;

	std::wstring type = DetectTypeDocument(srcTempPath);
	
	Oox2Odf::Converter converter(srcTempPath, type, L"C:\\Windows\\Fonts", NULL);

	converter.convert();
	converter.write(dstTempPath);

	FileSystem::Directory::DeleteDirectory(srcTempPath);

	if (hr != S_OK)  return hr;
   
	if (S_OK != oCOfficeUtils.CompressFileOrDirectory(dstTempPath.c_str(), dstPath.c_str(), -1))
        return hr;
	
	FileSystem::Directory::DeleteDirectory(dstTempPath);

////////////////////////////////////////////////////////////////////////
    std::cout << "\n\nTime : " << t1.elapsed() << "\n";   
	return 0;
}
