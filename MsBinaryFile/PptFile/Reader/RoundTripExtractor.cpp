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
#include "RoundTripExtractor.h"
#include "../../../DesktopEditor/common/Directory.h"
#include "../../../DesktopEditor/common/SystemUtils.h"
#include <boost/regex.hpp>

using namespace PPT;

RoundTripExtractor::RoundTripExtractor(const CUnknownRoundTrip* rt, const std::wstring& tempPath) : m_roundTripRecord(rt), m_hasError(false)
{
    m_tempPath = tempPath;
    m_hasError = !extract();
}

RoundTripExtractor::~RoundTripExtractor()
{
}

vector_string RoundTripExtractor::find(const std::wstring& strRegEx) const
{
    auto paths = NSDirectory::GetFiles(m_extractedFolderPath, true);
    std::vector<std::wstring> filtredPaths;
    try
    {
        boost::wregex regEx(strRegEx);
        boost::wsmatch wSmath;
        for (const auto& path : paths)
        {
            if (boost::regex_match(path, wSmath, regEx))
                filtredPaths.push_back(path);
        }
    } catch(...) {}

    return filtredPaths;
}

std::wstring RoundTripExtractor::getOneFile(const std::wstring &shortPath) const
{
    const std::wstring fullPath = m_extractedFolderPath + FILE_SEPARATOR_STR + shortPath;

    return NSFile::CFileBinary::Exists(fullPath) ? fullPath : L"";
}

bool RoundTripExtractor::extract()
{
    if (!m_roundTripRecord)
        return false;

    std::wstring tempZipPath = NSFile::CFileBinary::CreateTempFileWithUniqueName(m_tempPath, L"RndT") + L".zip";

    BYTE* zipData = m_roundTripRecord->data.first.get();
    ULONG zipDataLen = m_roundTripRecord->data.second;

    NSFile::CFileBinary binFile;
    binFile.CreateFileW(tempZipPath);
    binFile.WriteFile(zipData, zipDataLen);
    binFile.CloseFile();

    COfficeUtils officeUtils(NULL);
    m_extractedFolderPath = NSDirectory::CreateDirectoryWithUniqueName(m_tempPath);
    if(S_FALSE == officeUtils.ExtractToDirectory(tempZipPath, m_extractedFolderPath, NULL, 0))
        return false;
   
    NSFile::CFileBinary::Remove(tempZipPath);
    return true;
}

bool RoundTripExtractor::hasError() const
{
    return m_hasError;
}

std::string RoundTripExtractor::removeXMLAnnouncement(const std::string &strXMLFile)
{
    std::string strClearXML;
    auto iter = strXMLFile.find("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");


    return strClearXML;
}

std::string RoundTripExtractor::changeXMLTags(const std::string &strXML)
{
    return strXML;
}

std::wstring RoundTripExtractor::extractedFolderPath() const
{
    return m_extractedFolderPath;
}
