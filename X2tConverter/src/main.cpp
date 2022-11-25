/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
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

#include "cextracttools.h"
#include "ASCConverters.h"
#include "../../DesktopEditor/common/SystemUtils.h"
#include "../../DesktopEditor/common/StringExt.h"
#include "../../Common/3dParty/misc/proclimits.h"

#include <iostream>

#define VALUE_TO_STRING(x) #x
#define VALUE(x) VALUE_TO_STRING(x)

using namespace NExtractTools;

#if !defined(_WIN32) && !defined (_WIN64)
static std::wstring utf8_to_unicode(const char *src)
{
    return NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)src, (LONG)strlen(src));
}
#endif

#ifdef BUILD_X2T_AS_LIBRARY_DYLIB
#if !defined(_WIN32) && !defined (_WIN64)
    int main_lib(int argc, char *argv[])
#else
    int wmain_lib(int argc, wchar_t *argv[])
#endif
#endif
#ifndef BUILD_X2T_AS_LIBRARY_DYLIB
#if !defined(_WIN32) && !defined (_WIN64)
	int main(int argc, char *argv[])
#else
	int wmain(int argc, wchar_t *argv[])
#endif
#endif
{
//#define __CRTDBG_MAP_ALLOC
//#include <crtdbg.h>
//#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
//#define new DEBUG_NEW
		
// check arguments
    if (argc < 2)
    {
        // print out help topic

		std::cout << std::endl;
        std::cout << std::endl;
        std::cout << "-------------------------------------------------------------------------------" << std::endl;
        std::cout << "\t\tOOX/binary file converter. Version: " VALUE(INTVER)  << std::endl;
        std::cout << "-------------------------------------------------------------------------------" << std::endl;
        std::cout << std::endl;
		std::cout << "USAGE: x2t \"path_to_params_xml\"" << std::endl;
		std::cout << "or" << std::endl;
        std::cout << "USAGE: x2t \"path_to_file_1\" \"path_to_file_2\" [\"path_to_font_selection\"]" << std::endl;
        std::cout << "WHERE:" << std::endl;
        std::cout << "\t\"path_to_file_1\" is a path to file to be converted" << std::endl;
        std::cout << "\t\"path_to_file_2\" is a path to the corresponding output file" << std::endl;
        std::cout << "\t\"path_to_font_selection\" is a path to 'font_selection.bin' location" << std::endl << std::endl;
        std::cout << "NOTE: conversion direction will be calculated from file extensions" << std::endl << std::endl;

        return getReturnErrorCode(AVS_FILEUTILS_ERROR_CONVERT_PARAMS);
    }

	//set memory limit
	std::wstring sMemoryLimit = NSSystemUtils::GetEnvVariable(NSSystemUtils::gc_EnvMemoryLimit);
	if (sMemoryLimit.empty())
		sMemoryLimit = NSSystemUtils::gc_EnvMemoryLimitDefault;

#if !defined(_DEBUG)
	long long nMemoryLimit;
	if (NSStringExt::FromHumanReadableByteCount(sMemoryLimit, nMemoryLimit) && nMemoryLimit > 0)
		limit_memory((size_t)nMemoryLimit);
#endif
	std::wstring sArg1, sArg2, sExePath;

#if !defined(_WIN32) && !defined (_WIN64)
    sExePath    = utf8_to_unicode(argv [0]);
    sArg1       = utf8_to_unicode(argv [1]);
    if (argc >= 3) sArg2   = utf8_to_unicode(argv [2]);
#else
	sExePath	= std::wstring(argv [0]);
    sArg1		= std::wstring(argv [1]);
	if (argc >= 3) sArg2 = std::wstring(argv [2]);
#endif

	_UINT32 result = 0;
    std::wstring sXmlExt = _T(".xml");
    if((sArg1.length() > 3) && (sXmlExt == sArg1.substr(sArg1.length() - sXmlExt.length(), sXmlExt.length())))
	{
		NExtractTools::InputParams oInputParams;
		if (oInputParams.FromXmlFile(sArg1) && (sArg2.empty() || oInputParams.FromXml(sArg2)))
		{
			result = NExtractTools::fromInputParams(oInputParams);
		}
		else
		{
			result = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
		}
	}
	else
	{
		std::wstring sArg3, sArg4, sArg5;

#if !defined(_WIN32) && !defined (_WIN64)
        if (argc >= 4) sArg3   = utf8_to_unicode(argv [3]);
        if (argc >= 5) sArg4   = utf8_to_unicode(argv [4]);
        if (argc >= 6) sArg5   = utf8_to_unicode(argv [5]);
#else
		if (argc >= 4) sArg3 = std::wstring(argv [3]);
		if (argc >= 5) sArg4 = std::wstring(argv [4]);
		if (argc >= 6) sArg5 = std::wstring(argv [5]);
#endif
		if (sArg1 == L"-detectmacro")
		{
			InputParams oInputParams;
			oInputParams.m_sFileFrom	= new std::wstring(sArg2);
			
			result = NExtractTools::detectMacroInFile(oInputParams);
		}
		else if (sArg1 == L"-create-js-cache")
		{
			NExtractTools::createJSCaches();
			return 0;
		}
		else
		{
			InputParams oInputParams;
			oInputParams.m_sFileFrom	= new std::wstring(sArg1);
			oInputParams.m_sFileTo		= new std::wstring(sArg2);

			if (argc > 3)
			{
				oInputParams.m_sFontDir = new std::wstring(sArg3);
			}
			if (argc > 4)
			{
				oInputParams.m_sPassword = new std::wstring(sArg4);
				oInputParams.m_sSavePassword = new std::wstring(sArg4);
			}
			result = NExtractTools::fromInputParams(oInputParams);
		}
	}
	//_CrtDumpMemoryLeaks(); 
	return getReturnErrorCode(result);
}
