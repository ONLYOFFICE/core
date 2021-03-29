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

HRESULT CEpubFile::FromHtml(const std::wstring& sHtmlFile, const std::wstring& sCoreFile, const std::wstring& sDstFile)
{
    NSDirectory::CreateDirectory(m_sTempDir + L"/META-INF");
    NSDirectory::CreateDirectory(m_sTempDir + L"/OEBPS");
    NSDirectory::CreateDirectory(m_sTempDir + L"/OEBPS/images");
    // index.html
    std::wstring sIndexHtml;
    NSFile::CFileBinary::ReadAllTextUtf8(sHtmlFile, sIndexHtml);
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
    if (!NSFile::CFileBinary::Exists(m_sTempDir + L"/OEBPS/images/img1.png"))
    {
        NSFile::CFileBinary oCover;
        if (oCover.CreateFileW(m_sTempDir + L"/OEBPS/images/img1.png"))
        {
            DWORD nCoverLen = 1369;
            BYTE nCover[1369] = {137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,32,0,0,0,32,8,2,0,0,0,252,24,237,163,0,0,0,9,112,72,89,115,0,0,14,196,0,0,14,196,1,149,43,14,27,0,0,0,45,116,69,88,116,83,111,102,116,119,97,114,101,0,67,114,101,97,116,101,100,32,98,121,32,102,67,111,100,101,114,32,71,114,97,112,104,105,99,115,32,80,114,111,99,101,115,115,111,114,127,195,237,95,0,0,4,210,73,68,65,84,120,218,181,86,109,76,91,85,24,190,109,111,111,123,219,210,82,10,140,22,6,197,110,12,28,85,44,50,16,2,172,3,149,176,196,32,98,246,225,86,117,241,215,92,252,101,34,241,191,89,98,252,53,137,78,66,54,193,185,177,56,201,54,9,97,19,86,97,194,96,108,21,44,72,87,249,218,6,20,89,191,123,219,219,222,182,183,158,238,146,82,250,65,187,69,159,188,185,183,239,199,243,190,239,57,247,156,211,3,7,2,1,232,255,4,156,40,32,8,204,75,118,233,237,87,22,177,53,151,15,168,89,28,184,37,159,167,42,224,243,152,244,68,212,68,5,12,46,223,89,157,125,112,197,233,242,146,64,101,195,52,240,180,18,254,142,7,182,139,11,142,183,165,220,163,50,190,152,179,93,146,184,62,208,108,135,206,222,247,24,163,84,54,131,22,17,64,146,129,159,23,48,32,141,59,121,31,21,242,179,226,148,137,97,157,50,121,206,233,237,127,154,60,224,55,139,145,120,18,6,87,93,64,94,18,177,78,20,240,95,22,177,34,188,91,10,220,89,247,244,44,57,166,205,193,212,212,108,36,15,189,141,104,157,48,22,167,177,154,165,41,175,101,110,150,217,40,160,54,224,151,230,109,79,240,224,138,98,39,209,117,60,204,217,188,95,78,153,51,80,218,17,153,64,41,70,129,5,30,94,195,127,154,195,140,132,31,138,53,209,207,7,7,1,181,207,218,122,230,177,119,119,241,224,243,122,59,244,236,19,146,12,48,146,4,201,225,22,41,111,192,224,196,125,255,125,1,20,14,212,139,185,240,193,92,110,157,132,163,181,16,253,203,46,147,199,159,136,149,20,68,44,70,67,14,71,46,68,192,196,4,63,50,120,149,101,176,128,104,76,158,129,101,151,249,233,158,122,62,164,49,233,245,57,28,133,40,106,21,81,0,14,25,199,100,245,103,254,102,112,45,98,207,54,154,124,30,99,191,152,147,202,88,23,160,194,112,251,70,1,143,207,241,247,147,95,111,207,183,57,241,229,157,194,125,7,100,159,4,36,138,187,70,207,156,221,27,43,219,22,236,226,51,247,165,179,104,132,102,84,119,230,177,229,46,23,205,169,150,157,218,157,241,58,11,78,1,94,24,164,190,255,176,243,254,210,121,15,233,160,8,32,232,242,189,99,160,76,105,222,7,111,72,14,140,26,137,89,11,17,51,117,145,16,169,76,71,214,173,183,238,204,126,15,88,148,17,180,216,63,221,170,166,127,81,42,253,176,52,239,125,184,125,104,127,40,117,56,0,33,212,78,213,238,38,173,197,55,99,33,124,80,112,39,194,16,109,175,16,145,11,225,135,198,171,63,142,7,7,29,77,7,57,71,23,206,128,190,97,1,55,123,221,161,139,142,160,64,181,195,69,219,202,243,84,42,217,59,212,168,193,160,103,12,23,47,140,119,197,76,29,14,144,28,86,85,92,255,203,208,51,50,223,102,199,87,226,197,185,240,21,181,238,52,144,28,97,57,80,151,45,227,148,157,6,197,221,61,124,52,187,74,118,234,69,113,115,240,35,131,23,144,69,227,208,228,106,247,170,229,143,120,28,0,163,83,15,158,108,100,203,58,137,128,68,248,74,137,228,112,126,122,45,165,110,46,83,96,2,98,176,105,53,203,93,255,216,166,227,208,183,195,14,65,177,34,71,37,22,200,195,141,91,246,1,54,124,149,49,49,168,172,109,10,148,126,54,189,122,101,222,56,12,37,7,89,122,77,177,164,133,166,157,194,58,218,177,178,58,94,77,83,200,21,44,64,226,152,91,163,182,246,124,227,51,173,1,213,173,253,29,22,101,201,155,79,202,203,218,116,107,125,171,182,73,175,31,143,153,151,201,64,37,130,146,194,172,70,104,98,196,218,118,60,68,183,95,251,46,181,249,36,91,161,164,163,60,216,222,123,14,187,209,77,122,130,127,141,116,4,165,152,164,195,102,238,60,13,247,102,238,169,59,42,175,104,93,194,181,11,230,17,210,239,14,165,166,51,216,47,164,85,73,81,57,49,118,203,254,237,9,159,105,61,154,78,239,254,154,247,230,97,216,113,227,2,48,209,88,108,40,10,126,204,110,189,118,150,126,179,115,135,242,144,180,246,99,147,127,197,77,60,61,219,17,190,136,145,141,15,253,98,84,127,69,226,120,60,122,0,242,129,228,112,154,234,115,91,255,15,1,171,49,58,98,3,126,200,57,112,25,31,238,69,75,42,184,194,44,96,32,45,107,230,201,49,146,112,66,97,93,71,131,150,154,46,104,56,14,163,101,245,64,112,141,218,57,112,201,111,53,197,139,6,117,220,83,35,97,108,136,30,171,107,10,140,84,17,183,254,8,170,80,66,161,85,4,20,32,196,131,123,206,177,126,223,226,108,60,102,66,192,249,69,220,138,6,100,207,171,155,150,112,55,112,0,241,62,210,187,110,95,247,62,138,123,126,196,4,51,183,144,83,253,22,51,183,32,194,30,227,94,4,130,4,239,125,74,218,205,248,104,159,103,102,44,58,32,2,172,189,21,104,101,35,157,159,22,211,27,247,102,7,8,220,134,99,104,229,65,247,196,77,239,220,84,128,136,60,177,105,8,130,20,149,179,74,106,232,252,237,78,142,4,119,83,64,230,212,29,10,84,55,17,218,81,175,94,67,58,172,65,99,74,42,179,64,129,200,43,105,72,228,61,46,26,73,221,174,65,34,86,169,18,72,162,192,24,248,23,44,201,14,44,179,114,18,7,0,0,0,0,73,69,78,68,174,66,96,130};
            oCover.WriteFile(nCover, nCoverLen);
            oCover.CloseFile();
        }
    }
    std::wstring sHy, sWx;
    CBgraFrame oBgraFrame;
    if (oBgraFrame.OpenFile(m_sTempDir + L"/OEBPS/images/img1.png"))
    {
        sHy = std::to_wstring(oBgraFrame.get_Height());
        sWx = std::to_wstring(oBgraFrame.get_Width());
    }
    NSFile::CFileBinary oCoverHtml;
    if (oCoverHtml.CreateFileW(m_sTempDir + L"/OEBPS/cover.html"))
    {
        oCoverHtml.WriteStringUTF8(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?><!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.1//EN\" \"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\"><html xmlns=\"http://www.w3.org/1999/xhtml\"><head><title>Cover</title></head><body><div style=\"text-align: center; padding: 0pt; margin: 0pt;\"><svg xmlns=\"http://www.w3.org/2000/svg\" height=\"100%\" preserveAspectRatio=\"xMidYMid meet\" version=\"1.1\" width=\"100%\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" viewBox=\"0 0 ");
        oCoverHtml.WriteStringUTF8(sWx + L' ' + sHy + L"\"><image width=\"" + sWx + L"\" height=\"" + sHy);
        oCoverHtml.WriteStringUTF8(L"\" xlink:href=\"images/img1.png\"/></svg></div></body></html>");
        oCoverHtml.CloseFile();
    }
    // content.opf
    NSFile::CFileBinary oContentOpf;
    bool bWasLanguage = false;
    std::wstring sTitle = L"Title";
    std::wstring sUUID = GenerateUUID();
    if (oContentOpf.CreateFileW(m_sTempDir + L"/OEBPS/content.opf"))
    {
        oContentOpf.WriteStringUTF8(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><package xmlns=\"http://www.idpf.org/2007/opf\" version=\"2.0\" unique-identifier=\"book_uuid\"><metadata xmlns:dc=\"http://purl.org/dc/elements/1.1/\" xmlns:opf=\"http://www.idpf.org/2007/opf\">");
        // metadata
        std::wstring sCoreXml;
        bool bWasIdentifier = false;
        if (NSFile::CFileBinary::ReadAllTextUtf8(sCoreFile, sCoreXml))
        {
            XmlUtils::CXmlLiteReader oCoreReader;
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
                        size_t nBegin = sOut.find(L'>');
                        if (nBegin == std::wstring::npos)
                            continue;
                        sOut.erase(0, nBegin + 1);
                        nBegin = sOut.find(L'<');
                        if (nBegin == std::wstring::npos)
                            continue;
                        sOut.erase(nBegin);
                        sTitle = sOut;
                    }
                    else if (sName == L"dc:language")
                        bWasLanguage = true;
                }
            }
        }
        if (!bWasIdentifier)
        {
            oContentOpf.WriteStringUTF8(L"<dc:identifier id=\"book_uuid\" opf:scheme=\"UUID\">urn:uuid:");
            oContentOpf.WriteStringUTF8(sUUID);
            oContentOpf.WriteStringUTF8(L"</dc:identifier>");
        }
        oContentOpf.WriteStringUTF8(L"<meta name=\"cover\" content=\"img1.png\"/><dc:title>");
        oContentOpf.WriteStringUTF8(sTitle);
        oContentOpf.WriteStringUTF8(L"</dc:title>");
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
        oContentOpf.WriteStringUTF8(L"<item id=\"index\" href=\"index.html\" media-type=\"application/xhtml+xml\"/><item id=\"cover\" href=\"cover.html\" media-type=\"application/xhtml+xml\"/></manifest><spine toc=\"ncx\"><itemref idref=\"cover\"/><itemref idref=\"index\"/></spine><guide><reference type=\"cover\" title=\"Cover\" href=\"cover.html\"/></guide></package>");
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
