/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "common.h"

#include "../../../DesktopEditor/common/Directory.h"
#include "../../../DesktopEditor/common/Path.h"
#include "../../../OfficeUtils/src/OfficeUtils.h"
#include "../../../DesktopEditor/fontengine/ApplicationFontsWorker.h"
#include "../../../Common/OfficeFileFormatChecker.h"
#include "../../../DesktopEditor/common/StringBuilder.h"
#include "../../src/dylib/x2t.h"
#include "tchar.h"
#include <cstdlib> 
#include <locale>
#include <codecvt>
#include <boost/filesystem.hpp>


std::wstring GetWorkDir()
{
    std::wstring curDir = NSFile::GetProcessDirectory();
    return NSDirectory::CreateDirectoryWithUniqueName(curDir);
}

void RemoveWorkDir(const std::wstring &dir)
{
    NSDirectory::DeleteDirectory(dir);
}

int ConvertFile(const std::wstring &fileName)
{
    x2tchar* args[2];
	args[0] = NULL;
	args[1] = (x2tchar*)fileName.c_str();

	int nResultCode = X2T_Convert(2, args);
    return nResultCode;
}

void PrepareFiles(const std::wstring &fileName, const std::wstring &exampleFileName, const std::wstring &tempDirName)
{
    std::wstring sTempUnpackedXLSB = tempDirName + FILE_SEPARATOR_STR + _T("result_unpacked");
    NSDirectory::CreateDirectory(sTempUnpackedXLSB);
    COfficeUtils oCOfficeUtils(NULL);
    _UINT32 nRes = oCOfficeUtils.ExtractToDirectory(fileName, sTempUnpackedXLSB, NULL, 0);

    std::wstring sTempUnpackedXLSX = tempDirName + FILE_SEPARATOR_STR + _T("example_unpacked");
    NSDirectory::CreateDirectory(sTempUnpackedXLSX);
    nRes = oCOfficeUtils.ExtractToDirectory(exampleFileName, sTempUnpackedXLSX, NULL, 0);
}

std::wstring CreateParamsFile(const std::wstring &pathFrom, const std::wstring &pathTo, const std::wstring &TempDir)
{   
    NSStringUtils::CStringBuilder oBuilder;

	oBuilder.WriteString(L"<?xml version=\"1.0\" encoding=\"utf-8\"?>");
	oBuilder.WriteString(L"<TaskQueueDataConvert>");

	// main
	oBuilder.WriteString(L"<m_sFileFrom>");
	oBuilder.WriteEncodeXmlString(pathFrom);
	oBuilder.WriteString(L"</m_sFileFrom>");

	oBuilder.WriteString(L"<m_sFileTo>");
	oBuilder.WriteEncodeXmlString(pathTo);
	oBuilder.WriteString(L"</m_sFileTo>");

	// changes
	oBuilder.WriteString(L"<m_bFromChanges>false</m_bFromChanges>");

	oBuilder.WriteString(L"<m_bDontSaveAdditional>true</m_bDontSaveAdditional>");

	// temp directory
	oBuilder.WriteString(L"<m_sTempDir>");
	oBuilder.WriteEncodeXmlString(TempDir);
	oBuilder.WriteString(L"</m_sTempDir>");

	// txt/csv
	oBuilder.WriteString(L"<m_nCsvTxtEncoding>");
	oBuilder.WriteEncodeXmlString(L"46");
	oBuilder.WriteString(L"</m_nCsvTxtEncoding>");

	oBuilder.WriteString(L"<m_nCsvDelimiter>");
	oBuilder.WriteEncodeXmlString(L"4");
	oBuilder.WriteString(L"</m_nCsvDelimiter>");

	oBuilder.WriteString(L"</TaskQueueDataConvert>");

    auto xml = TempDir + FILE_SEPARATOR_STR + L"params.xml";
    // writing xml data into file
	if(NSFile::CFileBinary::Exists(xml))
		NSFile::CFileBinary::Remove(xml);

	NSFile::CFileBinary xml_file;
	xml_file.CreateFile(xml);
	xml_file.WriteStringUTF8(oBuilder.GetData());
	xml_file.CloseFile();

    return xml;
}

