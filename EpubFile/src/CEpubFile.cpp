#include "../CEpubFile.h"
#include "../../DesktopEditor/common/Directory.h"
#include "../../OfficeUtils/src/OfficeUtils.h"
#include "../../DesktopEditor/xml/include/xmlutils.h"
#include "../../HtmlFile2/htmlfile2.h"
#include "src/CBookInfo.h"

#include <iostream>
#include <string>

// Заменяет в строке s все символы s1 на s2
static void replace_all(std::wstring& s, const std::wstring& s1, const std::wstring& s2)
{
    size_t pos = s.find(s1);
    size_t l = s2.length();
    while(pos != std::string::npos)
    {
        s.replace(pos, s1.length(), s2);
        pos = s.find(s1, pos + l);
    }
}

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

    std::wstring sFileContent;
    std::wstring sContent;
    if(!NSFile::CFileBinary::ReadAllTextUtf8(m_sTempDir + L"/container.xml", sFileContent))
        return S_FALSE;
    size_t nContent = sFileContent.find(L"full-path");
    if(nContent != std::wstring::npos)
    {
        nContent += 11;
        sContent = sFileContent.substr(nContent, sFileContent.find(L'\"', nContent) - nContent);
        size_t posLastSlash = sContent.rfind(L'/');
        if (posLastSlash != std::wstring::npos)
            sContent = sContent.substr(posLastSlash + 1);
    }
    sContent = m_sTempDir + (sContent.empty() ? L"/content.opf" : L'/' + sContent);


    XmlUtils::CXmlLiteReader oXmlLiteReader;
    if (oXmlLiteReader.FromFile(sContent))
    {
        oXmlLiteReader.ReadNextNode();
        int nParentDepth = oXmlLiteReader.GetDepth();
        while (oXmlLiteReader.ReadNextSiblingNode(nParentDepth))
        {
            std::wstring sName = oXmlLiteReader.GetName();
            size_t nDot = sName.find(L':');
            if(nDot != std::wstring::npos)
                sName.erase(0, nDot + 1);
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
    oFileParams.SetPageBreakBefore(true);

    std::wstring sDocxFileTempDir = m_sTempDir + L"/tmp";
    NSDirectory::CreateDirectory(sDocxFileTempDir);
    oFile.SetTmpDirectory(sDocxFileTempDir);

    std::vector<std::wstring> arFiles;
    for (const CBookContentItem& oContent : m_arContents)
    {
        std::wstring sFile = m_mapRefs[oContent.m_sID].GetRef();
        replace_all(sFile, L"%20", L" ");
        arFiles.push_back(m_sTempDir + L"/" + sFile);
    }

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

HRESULT CEpubFile::FromHtml(const std::wstring& sInputFile, const std::wstring& sDstDirect)
{
    NSDirectory::CreateDirectory(sDstDirect + L"/META-INF");
    NSDirectory::CreateDirectory(sDstDirect + L"/OEBPS");

    NSFile::CFileBinary oMimeType;
    if(!oMimeType.CreateFileW(sDstDirect + L"/mimetype"))
        return S_FALSE;
    oMimeType.WriteStringUTF8(L"application/epub+zip");
    oMimeType.CloseFile();

    NSFile::CFileBinary oContainerXml;
    if(!oContainerXml.CreateFileW(sDstDirect + L"/META-INF/container.xml"))
        return S_FALSE;
    oContainerXml.WriteStringUTF8(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><container version=\"1.0\" xmlns=\"urn:oasis:names:tc:opendocument:xmlns:container\"><rootfiles><rootfile full-path=\"OEBPS/content.opf\" media-type=\"application/oebps-package+xml\"/></rootfiles></container>");
    oContainerXml.CloseFile();

    NSFile::CFileBinary oContentOpf;
    if(!oContentOpf.CreateFileW(sDstDirect + L"/OEBPS/content.opf"))
        return S_FALSE;
    oContentOpf.WriteStringUTF8(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><package><metadata></metadata><manifest></manifest><spine toc=\"ncx\"></spine><guide></guide></package>");
    oContentOpf.CloseFile();
}
