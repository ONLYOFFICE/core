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
void replace_all(std::wstring& s, const std::wstring& s1, const std::wstring& s2)
{
    size_t pos = s.find(s1);
    size_t l = s2.length();
    while (pos != std::string::npos)
    {
        s.replace(pos, s1.length(), s2);
        pos = s.find(s1, pos + l);
    }
}

std::wstring GenerateUUID()
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
    if (!NSFile::CFileBinary::ReadAllTextUtf8(m_sTempDir + L"/container.xml", sFileContent))
        return S_FALSE;
    size_t nContent = sFileContent.find(L"full-path");
    if (nContent != std::wstring::npos)
    {
        nContent += 11;
        sContent = sFileContent.substr(nContent, sFileContent.find(L'\"', nContent) - nContent);
        size_t posLastSlash = sContent.rfind(L'/');
        if (posLastSlash != std::wstring::npos)
            sContent = sContent.substr(posLastSlash + 1);
    }
    sContent = m_sTempDir + (sContent.empty() ? L"/content.opf" : L'/' + sContent);

    XmlUtils::CXmlLiteReader oXmlLiteReader;
    if (!oXmlLiteReader.FromFile(sContent))
        return S_FALSE;
    oXmlLiteReader.ReadNextNode();
    int nParentDepth = oXmlLiteReader.GetDepth();
    while (oXmlLiteReader.ReadNextSiblingNode(nParentDepth))
    {
        std::wstring sName = oXmlLiteReader.GetName();
        size_t nDot = sName.find(L':');
        if (nDot != std::wstring::npos)
            sName.erase(0, nDot + 1);
        if (sName == L"metadata")
            m_oBookInfo.ReadInfo(oXmlLiteReader);
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
    oFileParams.SetGenres (m_oBookInfo.GetSubjects());
    oFileParams.SetTitle  (m_oBookInfo.GetTitle());
    oFileParams.SetDate   (m_oBookInfo.GetDate());
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

#define DocInfo(name, tag)\
{\
    std::wstring sFind = L"<meta name=\""; sFind += name; sFind += L'\"';\
    if ((nFind = sIndexHtml.find(sFind)) != std::wstring::npos)\
    {\
        size_t nBegin = sIndexHtml.find(L"content=\"", nFind);\
        size_t nEnd   = sIndexHtml.find(L"\" />", nFind);\
        if (nBegin != std::wstring::npos && nEnd != std::wstring::npos && nBegin + 9 < nEnd)\
        {\
            nBegin += 9;\
            std::wstring sRes = L"<"; sRes += tag; sRes += L'>'; sRes += sIndexHtml.substr(nBegin, nEnd - nBegin); sRes += L"</"; sRes += tag; sRes += L">";\
            oContentOpf.WriteStringUTF8(sRes);\
        }\
    }\
}

HRESULT CEpubFile::FromHtml(const std::wstring& sHtmlFile, const std::wstring& sDstFile, const std::wstring& sInpTitle)
{
    NSDirectory::CreateDirectory(m_sTempDir + L"/META-INF");
    NSDirectory::CreateDirectory(m_sTempDir + L"/OEBPS");
    NSDirectory::CreateDirectory(m_sTempDir + L"/OEBPS/images");
    // index.html
    std::wstring sIndexHtml;
    NSFile::CFileBinary::ReadAllTextUtf8(sHtmlFile, sIndexHtml);
    // картинки в файл
    size_t nImage = sIndexHtml.find(L"data:image/png;base64, ");
    int nNumImage = 1;
    while (nImage != std::wstring::npos)
    {
        size_t nImageBegin = sIndexHtml.find(L' ', nImage) + 1;
        size_t nImageEnd = sIndexHtml.find(L'\"', nImageBegin);
        std::wstring sImage = sIndexHtml.substr(nImageBegin, nImageEnd - nImageBegin);
        NSFile::CFileBinary oImageWriter;
        if (oImageWriter.CreateFileW(m_sTempDir + L"/OEBPS/images/img" + std::to_wstring(nNumImage) + L".png"))
        {
            int nSrcLen = (int)sImage.length();
            int nDecodeLen = NSBase64::Base64DecodeGetRequiredLength(nSrcLen);
            BYTE* pImageData = new BYTE[nDecodeLen];
            if (TRUE == NSBase64::Base64Decode(U_TO_UTF8(sImage).c_str(), nSrcLen, pImageData, &nDecodeLen))
                oImageWriter.WriteFile(pImageData, (DWORD)nDecodeLen);
            RELEASEARRAYOBJECTS(pImageData);
            oImageWriter.CloseFile();
        }
        sIndexHtml.insert(sIndexHtml.find(L"/>", nImage), L" alt=\"NO\"");
        sIndexHtml.replace(nImage, nImageEnd - nImage, L"images/img" + std::to_wstring(nNumImage++) + L".png");
        nImage = sIndexHtml.find(L"data:image/png;base64, ", nImage);
    }
    // удаляем &nbsp;
    nImage = sIndexHtml.find(L"&nbsp;");
    while (nImage != std::wstring::npos)
    {
        sIndexHtml.replace(nImage, 6, L"&#160;");
        nImage = sIndexHtml.find(L"&nbsp;", nImage);
    }
    // заменяем <s> на style=text-decoration:line-through
    nImage = sIndexHtml.find(L"<s>");
    while (nImage != std::wstring::npos)
    {
        sIndexHtml.erase(nImage, 3);
        size_t nEndTag = sIndexHtml.find(L"</s>", nImage);
        sIndexHtml.erase(nEndTag, 4);
        nImage = sIndexHtml.rfind(L"style=\"", nImage);
        nEndTag = sIndexHtml.find(L"\"", nImage + 7);
        sIndexHtml.insert(nEndTag, L";text-decoration:line-through");
        nImage = sIndexHtml.find(L"<s>", nImage);
    }
    // удаляем атрибут width у <td>
    nImage = sIndexHtml.find(L"<td");
    while (nImage != std::wstring::npos)
    {
        size_t nEndTag = sIndexHtml.find(L">", nImage);
        nImage = sIndexHtml.find(L"width=\"", nImage);
        if (nImage < nEndTag)
        {
            nEndTag = sIndexHtml.find(L"\"", nImage + 7);
            sIndexHtml.erase(nImage, nEndTag - nImage + 1);
        }
        nImage = sIndexHtml.find(L"<td", nImage);
    }
    // удаляем атрибут clear у <br/>
    nImage = sIndexHtml.find(L"<br");
    while (nImage != std::wstring::npos)
    {
        size_t nEndTag = sIndexHtml.find(L">", nImage);
        nImage = sIndexHtml.find(L"clear=\"", nImage);
        if (nImage < nEndTag)
        {
            nEndTag = sIndexHtml.find(L"\"", nImage + 7);
            sIndexHtml.erase(nImage, nEndTag - nImage + 1);
        }
        nImage = sIndexHtml.find(L"<br", nImage);
    }
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
    // content.opf
    NSFile::CFileBinary oContentOpf;
    bool bWasLanguage = false, bWasTitle = false;
    std::wstring sTitle = sInpTitle.empty() ? NSFile::GetFileName(sDstFile) : sInpTitle;
    std::wstring sUUID = GenerateUUID();
    if (oContentOpf.CreateFileW(m_sTempDir + L"/OEBPS/content.opf"))
    {
        oContentOpf.WriteStringUTF8(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><package xmlns=\"http://www.idpf.org/2007/opf\" version=\"2.0\" unique-identifier=\"book_uuid\"><metadata xmlns:dc=\"http://purl.org/dc/elements/1.1/\" xmlns:dcterms=\"http://purl.org/dc/terms/\" xmlns:cp=\"http://schemas.openxmlformats.org/package/2006/metadata/core-properties\" xmlns:opf=\"http://www.idpf.org/2007/opf\">");
        // metadata
        bool bWasIdentifier = false;

        size_t nFind;
        if ((nFind = sIndexHtml.find(L"<title>")) != std::wstring::npos)
        {
            size_t nEnd = sIndexHtml.find(L"</title>", nFind);
            if (nEnd != std::wstring::npos)
            {
                bWasTitle = true;
                sTitle = sIndexHtml.substr(nFind + 7, nEnd - nFind - 7);
                oContentOpf.WriteStringUTF8(L"<dc:title>" + sTitle + L"</dc:title>");
            }
        }
        DocInfo(L"identifier", L"dc:identifier");
        DocInfo(L"language", L"dc:language");
        DocInfo(L"creator", L"dc:creator");
        DocInfo(L"description", L"dc:description");
        DocInfo(L"subject", L"dc:subject");
        DocInfo(L"keywords", L"cp:keywords");
        bWasIdentifier = sIndexHtml.find(L"<meta name=\"identifier\"") != std::wstring::npos;
        bWasLanguage = sIndexHtml.find(L"<meta name=\"language\"") != std::wstring::npos;

        if (!bWasIdentifier)
        {
            oContentOpf.WriteStringUTF8(L"<dc:identifier id=\"book_uuid\" opf:scheme=\"UUID\">urn:uuid:");
            oContentOpf.WriteStringUTF8(sUUID);
            oContentOpf.WriteStringUTF8(L"</dc:identifier>");
        }
        if (!bWasTitle)
        {
            oContentOpf.WriteStringUTF8(L"<dc:title>");
            oContentOpf.WriteStringUTF8(sTitle);
            oContentOpf.WriteStringUTF8(L"</dc:title>");
        }
        if (!bWasLanguage)
            oContentOpf.WriteStringUTF8(L"<dc:language>en-EN</dc:language>");
        // manifest
        oContentOpf.WriteStringUTF8(L"</metadata><manifest><item id=\"ncx\" href=\"toc.ncx\" media-type=\"application/x-dtbncx+xml\"/>");
        std::vector<std::wstring> arFiles = NSDirectory::GetFiles(m_sTempDir + L"/OEBPS/images");
        for (const std::wstring& sFileName : arFiles)
        {
            std::wstring sName = NSFile::GetFileName(sFileName);
            oContentOpf.WriteStringUTF8(L"<item id=\"" + sName + L"\" href=\"images/" + sName + L"\" media-type=\"image/png\"/>");
        }
        // spine & guide
        oContentOpf.WriteStringUTF8(L"<item id=\"index\" href=\"index.html\" media-type=\"application/xhtml+xml\"/></manifest><spine toc=\"ncx\"><itemref idref=\"index\"/></spine></package>");
        oContentOpf.CloseFile();
    }
    // toc.ncx
    NSFile::CFileBinary oTocNcx;
    if (oTocNcx.CreateFileW(m_sTempDir + L"/OEBPS/toc.ncx"))
    {
        oTocNcx.WriteStringUTF8(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE ncx PUBLIC \"-//NISO//DTD ncx 2005-1//EN\" \"http://www.daisy.org/z3986/2005/ncx-2005-1.dtd\"><ncx xmlns=\"http://www.daisy.org/z3986/2005/ncx/\" version=\"2005-1\"><head><meta name=\"dtb:uid\" content=\"urn:uuid:");
        oTocNcx.WriteStringUTF8(sUUID);
        oTocNcx.WriteStringUTF8(L"\"/><meta name=\"dtb:depth\" content=\"0\"/><meta name=\"dtb:totalPageCount\" content=\"0\"/><meta name=\"dtb:maxPageNumber\" content=\"0\"/></head><docTitle><text>");
        oTocNcx.WriteStringUTF8(sTitle);
        oTocNcx.WriteStringUTF8(L"</text></docTitle><navMap><navPoint id=\"navPoint-1\" playOrder=\"1\"><navLabel><text>Start</text></navLabel><content src=\"index.html\"/></navPoint></navMap></ncx>");
        oTocNcx.CloseFile();
    }
    // write index.html
    sIndexHtml.erase(0, 6);
    if (sIndexHtml.find(L"<title>") == std::wstring::npos)
        sIndexHtml.insert(sIndexHtml.find(L"</head>"), L"<title>" + sTitle + L"</title>");
    NSFile::CFileBinary oIndexHtml;
    if (oIndexHtml.CreateFileW(m_sTempDir + L"/OEBPS/index.html"))
    {
        oIndexHtml.WriteStringUTF8(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.1//EN\" \"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\"><html xmlns=\"http://www.w3.org/1999/xhtml\">");
        oIndexHtml.WriteStringUTF8(sIndexHtml);
        oIndexHtml.CloseFile();
    }
    // compress
    COfficeUtils oOfficeUtils;
    return oOfficeUtils.CompressFileOrDirectory(m_sTempDir, sDstFile);
}
