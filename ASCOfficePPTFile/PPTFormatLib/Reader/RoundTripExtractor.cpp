#include "RoundTripExtractor.h"
#include "../../../DesktopEditor/common/Directory.h"
#include "../../../DesktopEditor/common/SystemUtils.h"
#include <boost/regex.hpp>

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
