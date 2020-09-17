#include "../CEpubFile.h"
#include "../../DesktopEditor/common/Directory.h"
#include "../../OfficeUtils/src/OfficeUtils.h"
#include "../../DesktopEditor/xml/include/xmlutils.h"
#include "../../HtmlFile2/htmlfile2.h"
#include "src/CBookInfo.h"
#include <iostream>

CEpubFile::CEpubFile()
{
}

CEpubFile::~CEpubFile()
{
    if (!m_sTempDir.empty())
        NSDirectory::DeleteDirectory(m_sTempDir);
}

HRESULT CEpubFile::IsEpubFile(const std::wstring &sFileName)
{
    const auto& posPoint = sFileName.find_last_of(L'.');

    if (posPoint == std::wstring::npos ||
        sFileName.substr(posPoint + 1) != L"epub")
        return S_FALSE;

    COfficeUtils oOfficeUtils;

    if (oOfficeUtils.IsArchive(sFileName) == S_OK &&
        oOfficeUtils.IsFileExistInArchive(sFileName, L"META-INF/container.xml") == S_OK)
    {
        m_sFileName = sFileName;
        return S_OK;
    }
    return S_FALSE;
}

void CEpubFile::SetTempDirectory(const std::wstring &sTempDir)
{
    this->m_sTempDir = sTempDir;
}

HRESULT CEpubFile::Convert(const std::wstring& sInputFile, const std::wstring& sOutputFile, const bool& bIsOutCompress)
{
    bool bIsOwnTmp = false;
    if (m_sTempDir.empty())
    {
        bIsOwnTmp = true;
        NSDirectory::CreateDirectories(NSFile::GetProcessDirectory() + L"/TEMP");
        SetTempDirectory(NSFile::GetProcessDirectory() + L"/TEMP");
    }
    else
    {
        NSDirectory::CreateDirectories(m_sTempDir);
        SetTempDirectory(m_sTempDir);
    }

    COfficeUtils oOfficeUtils;

    wchar_t* password = NULL;
    if (oOfficeUtils.ExtractToDirectory(sInputFile, m_sTempDir.c_str(), password, 1) != S_OK)
        return S_FALSE;

    XmlUtils::CXmlLiteReader oXmlLiteReader;

    if (oXmlLiteReader.FromFile(m_sTempDir + L"/content.opf"))
    {
        oXmlLiteReader.ReadNextNode();

        const int& nParentDepth = oXmlLiteReader.GetDepth();

        while (oXmlLiteReader.ReadNextSiblingNode(nParentDepth))
        {
            const std::wstring& sName = oXmlLiteReader.GetName();

            if (sName == L"metadata")
            {
                m_oBookInfo.ReadInfo(oXmlLiteReader);
//                #ifdef _DEBUG
//                    m_oBookInfo.ShowInfo();
//                #endif
            }
            else if (sName == L"manifest")
            {
                const int& _nParentDepth = oXmlLiteReader.GetDepth();

                while (true)
                {
                    CBookItem oItem;
                    if (oItem.ReadItem(oXmlLiteReader, _nParentDepth))
                        m_mapRefs.emplace(oItem.GetID(), oItem);
                    else
                        break;
                }
            }
            else if (sName == L"spine")
            {
                const int& _nParentDepth = oXmlLiteReader.GetDepth();

                while (true)
                {
                    CBookContentItem oContentItem;
                    if (oContentItem.ReadContentItem(oXmlLiteReader, _nParentDepth))
                        m_arContents.push_back(oContentItem);
                    else
                        break;
                }
            }
        }
    }
    else
        return S_FALSE;

    HRESULT hRes = S_FALSE;
    if (oXmlLiteReader.FromFile(m_sTempDir + L"/toc.ncx"))
    {
        oXmlLiteReader.ReadNextNode();
        m_oToc.ReadToc(oXmlLiteReader);
//        #ifdef _DEBUG
//            m_oToc.ShowToc();
//        #endif

        CHtmlFile2 oFile;
        CHtmlParams oFileParams;

        oFileParams.SetAuthors(m_oBookInfo.GetCreators());
        oFileParams.SetGenres(m_oBookInfo.GetSubjects());
        oFileParams.SetTitle(m_oBookInfo.GetTitle());
        oFileParams.SetDate(m_oBookInfo.GetDate());
        oFileParams.SetDescription(m_oBookInfo.GetDescriptions());

        std::wstring sDocxFileTempDir = m_sTempDir + L"/tmp";
        NSDirectory::CreateDirectory(sDocxFileTempDir);
        oFile.SetTmpDirectory(sDocxFileTempDir);

        std::vector<std::wstring> arFiles;
        for (const CBookContentItem& oContent : m_arContents)
            arFiles.push_back(m_sTempDir + L"/" + m_mapRefs[oContent.m_sID].GetRef());

#ifdef _DEBUG
        std::wcout << L"---The conversion process from Epub to Docx...---" << std::endl;
#endif

        if (bIsOutCompress)
        {
            std::wstring sOutputDir = m_sTempDir + L"/res";
            NSDirectory::CreateDirectory(sOutputDir);

            hRes = oFile.OpenBatchHtml(arFiles, sOutputDir, &oFileParams);

            if (S_OK == hRes)
                oOfficeUtils.CompressFileOrDirectory(sOutputDir, sOutputFile);
        }
        else
        {
            hRes = oFile.OpenBatchHtml(arFiles, sOutputFile, &oFileParams);
        }

#ifdef _DEBUG
        if (S_OK == hRes)
            std::wcout << L"---Successful conversion of Epub to Docx---" << std::endl;
        else
            std::wcout << L"---Failed conversion of Epub to Docx---" << std::endl;
#endif
    }

    if (!bIsOwnTmp)
        m_sTempDir = L"";

    return hRes;
}

void CEpubFile::Clear()
{
    m_sFileName.clear();
    m_sTempDir.clear();
    m_oBookInfo.Clear();
    m_mapRefs.clear();
    m_oToc.Clear();
    m_arContents.clear();

    if (!m_sTempDir.empty())
        NSDirectory::DeleteDirectory(m_sTempDir);
}

void CEpubFile::ShowMap()
{
    std::cout << "-----MAP-----" << std::endl;
    for (const CBookContentItem& oItem : m_arContents)
        std::wcout << oItem.m_sID << L" - " << m_mapRefs[oItem.m_sID].GetRef() << std::endl;
}
