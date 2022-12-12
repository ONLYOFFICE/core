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
#include "RoundTripExtractor.h"
#include "../../../DesktopEditor/common/Directory.h"
#include "../../../DesktopEditor/common/SystemUtils.h"
#include <boost/regex.hpp>

using namespace PPT;

RoundTripExtractor::RoundTripExtractor(const CUnknownRoundTrip* rt) :
    m_roundTripRecord(rt), m_hasError(false)
{
    m_hasError = !extract();
}

RoundTripExtractor::~RoundTripExtractor()
{
    NSDirectory::DeleteDirectory(m_extractedFolderPath);
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


    std::wstring tempRootPath = NSDirectory::GetTempPath();
    if (false == NSDirectory::Exists(tempRootPath))
        return false;
    std::wstring tempZipPath = tempRootPath + FILE_SEPARATOR_STR + L"RoundTrip.zip";


    BYTE* zipData = m_roundTripRecord->data.first.get();
    ULONG zipDataLen = m_roundTripRecord->data.second;

    NSFile::CFileBinary binFile;
    binFile.CreateFileW(tempZipPath);
    binFile.WriteFile(zipData, zipDataLen);
    binFile.CloseFile();

    COfficeUtils officeUtils(NULL);
    m_extractedFolderPath = NSDirectory::CreateDirectoryWithUniqueName(tempRootPath);
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
