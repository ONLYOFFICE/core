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
}

HRESULT CEpubFile::IsEpubFile(const std::wstring& sFileName)
{
    if (NSFile::GetFileExtention(sFileName) != L"epub")
        return S_FALSE;

    COfficeUtils oOfficeUtils;

    m_sFileName = sFileName;
    return oOfficeUtils.IsFileExistInArchive(sFileName, L"META-INF/container.xml") == S_OK ? S_OK : S_FALSE;
}

void CEpubFile::SetTempDirectory(const std::wstring& sTempDir)
{
    m_sTempDir = sTempDir;
}

HRESULT CEpubFile::Convert(const std::wstring& sInputFile, const std::wstring& sOutputFile, bool bIsOutCompress)
{
    COfficeUtils oOfficeUtils;

    wchar_t* password = NULL;
    if (oOfficeUtils.ExtractToDirectory(sInputFile, m_sTempDir.c_str(), password, 1) != S_OK)
        return S_FALSE;

    XmlUtils::CXmlLiteReader oXmlLiteReader;

    if (oXmlLiteReader.FromFile(m_sTempDir + L"/content.opf"))
    {
        oXmlLiteReader.ReadNextNode();
        int nParentDepth = oXmlLiteReader.GetDepth();
        while (oXmlLiteReader.ReadNextSiblingNode(nParentDepth))
        {
            std::wstring sName = oXmlLiteReader.GetName();
            if (sName == L"metadata")
            {
                m_oBookInfo.ReadInfo(oXmlLiteReader);
//                #ifdef _DEBUG
//                    m_oBookInfo.ShowInfo();
//                #endif
            }
            else if (sName == L"manifest")
            {
                int _nParentDepth = oXmlLiteReader.GetDepth();
                while (true)
                {
                    CBookItem oItem;
                    if (oItem.ReadItem(oXmlLiteReader, _nParentDepth))
                        m_mapRefs.insert(std::make_pair(oItem.GetID(), oItem));
                    else
                        break;
                }
            }
            else if (sName == L"spine")
            {
                int _nParentDepth = oXmlLiteReader.GetDepth();
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

    /*
    if (!oXmlLiteReader.FromFile(m_sTempDir + L"/toc.ncx"))
        return S_FALSE;

    oXmlLiteReader.ReadNextNode();
    m_oToc.ReadToc(oXmlLiteReader);
//        #ifdef _DEBUG
//            m_oToc.ShowToc();
//        #endif
    */

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

    std::wstring sOutputDir = m_sTempDir + L"/res";
    if (!bIsOutCompress)
        sOutputDir = sOutputFile;

    NSDirectory::CreateDirectory(sOutputDir);
    HRESULT hRes = oFile.OpenBatchHtml(arFiles, sOutputDir, &oFileParams);
    if (bIsOutCompress && S_OK == hRes)
        oOfficeUtils.CompressFileOrDirectory(sOutputDir, sOutputFile);

#ifdef _DEBUG
    std::wcout << L"---" << (S_OK == hRes ? L"Successful" : L"Failed") << L" conversion of Epub to Docx---" << std::endl;
#endif

    return hRes;
}

void CEpubFile::Clear()
{
    m_sFileName .clear();
    m_sTempDir  .clear();
    m_oBookInfo .Clear();
    m_mapRefs   .clear();
    m_oToc      .Clear();
    m_arContents.clear();
}

void CEpubFile::ShowMap()
{
    std::cout << "-----MAP-----" << std::endl;
    for (const CBookContentItem& oItem : m_arContents)
        std::wcout << oItem.m_sID << L" - " << m_mapRefs[oItem.m_sID].GetRef() << std::endl;
}
