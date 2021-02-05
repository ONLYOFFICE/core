#include "../CEpubFile.h"
#include "../../DesktopEditor/common/Directory.h"
#include "../../OfficeUtils/src/OfficeUtils.h"
#include "../../DesktopEditor/xml/include/xmlutils.h"
#include "../../HtmlFile2/htmlfile2.h"
#include "../../DesktopEditor/raster/BgraFrame.h"
#include "src/CBookInfo.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <random>

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

static std::wstring GenerateUUID()
{
    std::mt19937 oRand(time(0));
    std::wstringstream sstream;
    sstream << std::setfill(L'0') << std::hex << std::setw(8) << (oRand() & 0xffffffff);
    sstream << L'-';
    sstream << std::setfill(L'0') << std::hex << std::setw(4) << (oRand() & 0xffff);
    sstream << L'-';
    sstream << std::setfill(L'0') << std::hex << std::setw(4) << (oRand() & 0xffff);
    sstream << L'-';
    sstream << std::setfill(L'0') << std::hex << std::setw(4) << (oRand() & 0xffff);
    sstream << L'-';
    sstream << std::setfill(L'0') << std::hex << std::setw(8) << (oRand() & 0xffffffff);
    return sstream.str();
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

HRESULT CEpubFile::FromHtml(const std::wstring& sSrc, const std::wstring& sDstFile)
{
    NSDirectory::CreateDirectory(m_sTempDir + L"/META-INF");
    NSDirectory::CreateDirectory(m_sTempDir + L"/OEBPS");
    NSFile::CFileBinary::Copy(sSrc + L"/doct_unpacked/index.html", m_sTempDir + L"/OEBPS/index.html");
    NSDirectory::CopyDirectory(sSrc + L"/doct_unpacked/images", m_sTempDir + L"/OEBPS/images");
    // mimetype
    NSFile::CFileBinary oMimeType;
    if (oMimeType.CreateFileW(m_sTempDir + L"/mimetype"))
    {
        oMimeType.WriteStringUTF8(L"application/epub+zip");
        oMimeType.CloseFile();
    }
    // container.xml
    NSFile::CFileBinary oContainerXml;
    if (oContainerXml.CreateFileW(m_sTempDir + L"/META-INF/container.xml"))
    {
        oContainerXml.WriteStringUTF8(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><container version=\"1.0\" xmlns=\"urn:oasis:names:tc:opendocument:xmlns:container\"><rootfiles><rootfile full-path=\"OEBPS/content.opf\" media-type=\"application/oebps-package+xml\"/></rootfiles></container>");
        oContainerXml.CloseFile();
    }
    // cover.html
    bool bCoverFromImg1 = NSFile::CFileBinary::Exists(sSrc + L"/doct_unpacked/images/img1.png");
    if (bCoverFromImg1)
    {
        int nHy = 0;
        int nWx = 0;
        CBgraFrame oBgraFrame;
        if (oBgraFrame.OpenFile(m_sTempDir + L"/OEBPS/images/img1.png"))
        {
            nHy = oBgraFrame.get_Height();
            nWx = oBgraFrame.get_Width();
        }
        NSFile::CFileBinary oCoverHtml;
        if (oCoverHtml.CreateFileW(m_sTempDir + L"/OEBPS/cover.html"))
        {
            oCoverHtml.WriteStringUTF8(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?><html xmlns=\"http://www.w3.org/1999/xhtml\"><head><title>Cover</title></head><body><div style=\"text-align: center; padding: 0pt; margin: 0pt;\"><svg xmlns=\"http://www.w3.org/2000/svg\" height=\"100%\" preserveAspectRatio=\"xMidYMid meet\" version=\"1.1\" width=\"100%\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" viewBox=\"0 0 " + std::to_wstring(nWx) + L' ' + std::to_wstring(nHy) + L"\"><image width=\"" + std::to_wstring(nWx) + L"\" height=\"" + std::to_wstring(nHy) + L"\" xlink:href=\"images/img1.png\"/></svg></div></body></html>");
            oCoverHtml.CloseFile();
        }
    }
    // content.opf
    NSFile::CFileBinary oContentOpf;
    std::wstring sTitle;
    std::wstring sUUID = GenerateUUID();
    if (oContentOpf.CreateFileW(m_sTempDir + L"/OEBPS/content.opf"))
    {
        oContentOpf.WriteStringUTF8(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><package xmlns=\"http://www.idpf.org/2007/opf\" version=\"2.0\" unique-identifier=\"book_uuid\"><metadata xmlns:dc=\"http://purl.org/dc/elements/1.1/\" xmlns:opf=\"http://www.idpf.org/2007/opf\">");
        // metadata
        std::wstring sCoreXml;
        bool bWasIdentifier = false;
        XmlUtils::CXmlLiteReader oCoreReader;
        if (NSFile::CFileBinary::ReadAllTextUtf8(sSrc + L"/docx_unpacked/docProps/core.xml", sCoreXml))
        {
            oCoreReader.FromString(sCoreXml);
            oCoreReader.ReadNextNode();
            int nDeath = oCoreReader.GetDepth();
            while (oCoreReader.ReadNextSiblingNode(nDeath))
            {
                if (oCoreReader.GetNamespacePrefix() == L"dc")
                {
                    std::wstring sOut = oCoreReader.GetOuterXml();
                    oContentOpf.WriteStringUTF8(sOut);
                    std::wstring sName = oCoreReader.GetName();
                    if (sName == L"dc:identifier")
                        bWasIdentifier = true;
                    else if (sName == L"dc:title")
                    {
                        sOut.erase(0, sOut.find(L'>') + 1);
                        sOut.erase(sOut.find(L'<'));
                        sTitle = sOut;
                    }
                }
            }
        }
        if (!bWasIdentifier)
        {
            oContentOpf.WriteStringUTF8(L"<dc:identifier id=\"book_uuid\" opf:scheme=\"UUID\">urn:uuid:");
            oContentOpf.WriteStringUTF8(sUUID);
            oContentOpf.WriteStringUTF8(L"</dc:identifier>");
        }
        if (bCoverFromImg1)
            oContentOpf.WriteStringUTF8(L"<meta name=\"cover\" content=\"img1.png\"/>");
        // manifest
        oContentOpf.WriteStringUTF8(L"</metadata><manifest><item id=\"ncx\" href=\"toc.ncx\" media-type=\"application/x-dtbncx+xml\"/>");
        std::vector<std::wstring> arFiles = NSDirectory::GetFiles(m_sTempDir + L"/OEBPS/images");
        for (const std::wstring& sFileName : arFiles)
        {
            std::wstring sName = NSFile::GetFileName(sFileName);
            oContentOpf.WriteStringUTF8(L"<item id=\"" + sName + L"\" href=\"images/" + sName + L"\" media-type=\"image/png\"/>");
        }
        oContentOpf.WriteStringUTF8(L"<item id=\"index\" href=\"index.html\" media-type=\"application/xhtml+xml\"/>");
        if (bCoverFromImg1)
            oContentOpf.WriteStringUTF8(L"<item id=\"cover\" href=\"cover.html\" media-type=\"application/xhtml+xml\"/>");
        // spine
        oContentOpf.WriteStringUTF8(L"</manifest><spine toc=\"ncx\">");
        if (bCoverFromImg1)
            oContentOpf.WriteStringUTF8(L"<itemref idref=\"cover\"/>");
        // guide
        oContentOpf.WriteStringUTF8(L"<itemref idref=\"index\"/></spine><guide>");
        if (bCoverFromImg1)
            oContentOpf.WriteStringUTF8(L"<reference type=\"cover\" title=\"Cover\" href=\"cover.html\"/>");
        oContentOpf.WriteStringUTF8(L"</guide></package>");
        oContentOpf.CloseFile();
    }
    // toc.ncx
    NSFile::CFileBinary oTocNcx;
    if (oTocNcx.CreateFileW(m_sTempDir + L"/OEBPS/toc.ncx"))
    {
        oTocNcx.WriteStringUTF8(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><ncx xmlns=\"http://www.daisy.org/z3986/2005/ncx/\" version=\"2005-1\"><head><meta name=\"dtb:uid\" content=\"urn:uuid:" + sUUID + L"\"/><meta name=\"dtb:depth\" content=\"0\"/><meta name=\"dtb:totalPageCount\" content=\"0\"/><meta name=\"dtb:maxPageNumber\" content=\"0\"/></head><docTitle><text>" + sTitle + L"</text></docTitle><navMap><navPoint id=\"navPoint-1\" playOrder=\"1\"><navLabel><text>Start</text></navLabel><content src=\"cover.html\"/></navPoint></navMap></ncx>");
        oTocNcx.CloseFile();
    }
    COfficeUtils oOfficeUtils;
    return oOfficeUtils.CompressFileOrDirectory(m_sTempDir, sDstFile);
}
