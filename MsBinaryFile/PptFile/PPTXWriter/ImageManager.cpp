#include "ImageManager.h"


PPT_FORMAT::CMediaManager::CMediaManager() : m_lIndexNextImage(0), m_lIndexNextAudio(0), m_lIndexNextVideo(0)
{
}

PPT_FORMAT::CMediaManager::~CMediaManager()
{
}

void PPT_FORMAT::CMediaManager::Clear()
{
    m_mapMedia.clear();

    m_lIndexNextImage = 0;
    m_lIndexNextAudio = 0;
    m_lIndexNextVideo = 0;
}

std::wstring PPT_FORMAT::CMediaManager::FindMedia(const std::wstring &strInput)
{
    std::map<std::wstring, std::wstring>::iterator pPair = m_mapMedia.find(strInput);
    if (m_mapMedia.end() != pPair)
    {
        return pPair->second;
    }
    return L"";
}

void PPT_FORMAT::CMediaManager::SetDstMedia(const std::wstring &strDst)
{
    m_strDstMedia = strDst;
}

std::wstring PPT_FORMAT::CMediaManager::GenerateVideo(const std::wstring &strInput)
{
    return GenerateMedia(strInput, L"video", m_lIndexNextVideo, L".avi");
}

std::wstring PPT_FORMAT::CMediaManager::GenerateAudio(const std::wstring &strInput)
{
    return GenerateMedia(strInput, L"audio", m_lIndexNextAudio, L".wav");
}

std::wstring PPT_FORMAT::CMediaManager::GenerateImage(const std::wstring &strInput)
{
    return GenerateMedia(strInput, L"image", m_lIndexNextImage, L".png");
}

std::wstring PPT_FORMAT::CMediaManager::GenerateImageJPEG(const std::wstring &strInput)
{
    return GenerateMedia(strInput, L"image", m_lIndexNextImage, L".jpeg");
}

std::wstring PPT_FORMAT::CMediaManager::GenerateMedia(const std::wstring &strInput, const std::wstring &Template, long &Indexer, const std::wstring &strDefaultExt)
{
    std::map<std::wstring, std::wstring>::iterator pPair = m_mapMedia.find(strInput);
    if (m_mapMedia.end() != pPair)
    {
        return pPair->second;
    }

    if (IsNeedDownload(strInput))
    {
#ifndef DISABLE_FILE_DOWNLOADER
        NSNetwork::NSFileTransport::CFileDownloader oDownloader(strInput, TRUE);
        if ( oDownloader.DownloadSync() )
        {
            std::wstring file_name = oDownloader.GetFilePath();

            //todooo - check media file
            return GenerateMedia(file_name , Template, Indexer, strDefaultExt);
        }
#endif
    }

    std::wstring strExts = strDefaultExt;
    int nIndexExt = strInput.rfind(wchar_t('.'));
    if (-1 != nIndexExt)
        strExts = strInput.substr(nIndexExt);

    if (strExts == _T(".video") || strExts == _T(".audio"))
    {
        std::wstring strInput1 = strInput.substr(0, nIndexExt);
        nIndexExt = strInput1.rfind(wchar_t('.'));
        strExts =  nIndexExt < 0 ? L"" : strInput1.substr(nIndexExt);
    }
    if (strExts == _T(".tmp") || strExts.empty()) strExts = strDefaultExt;

    std::wstring strMediaName = Template + std::to_wstring(++Indexer);

    std::wstring strOutput = m_strDstMedia + strMediaName + strExts;
    strMediaName  = _T("../media/") + strMediaName + strExts;

    // теперь нужно скопировать
    if (strOutput != strInput)
    {
        if (NSFile::CFileBinary::Copy(strInput, strOutput) == false)
        {
            return L"";
        }
    }
    m_mapMedia[strInput] = strMediaName;
    return strMediaName;
}

void PPT_FORMAT::CMediaManager::WriteAudioCollection(const std::vector<CExFilesInfo> &audioCont)
{
    if (audioCont.empty()) return;

    for (auto& audio : audioCont)
    {
        auto pathAudio = GenerateAudio(audio.m_strFilePath);
    }

}

bool PPT_FORMAT::CMediaManager::IsNeedDownload(const std::wstring &strFile)
{
    int n1 = strFile.find(L"www");
    int n2 = strFile.find(L"http");
    int n3 = strFile.find(L"ftp");

    if (((n1 >= 0) && (n1 < 10)) || ((n2 >= 0) && (n2 < 10)) || ((n3 >= 0) && (n3 < 10)))
        return true;
    return false;
}

std::wstring PPT_FORMAT::CorrectXmlString3(const std::wstring &str)
{
    std::wstring buffer;
    buffer.reserve(str.size());
    for(size_t pos = 0; pos != str.size(); ++pos)
    {
        switch(str[pos])
        {
        case '&':  buffer.append(_T("&amp;"));      break;
        case '\"': buffer.append(_T("&quot;"));     break;
        case '\'': buffer.append(_T("&apos;"));     break;
        case '<':  buffer.append(_T("&lt;"));       break;
        case '>':  buffer.append(_T("&gt;"));       break;
        default:   buffer.append(&str[pos], 1);	break;
        }
    }
    return buffer;
}

PPT_FORMAT::CRelsGenerator::CRelsGenerator(CMediaManager *pManager) : m_oWriter(), m_lNextRelsID(1)
{
    m_pManager = pManager;
}

PPT_FORMAT::CRelsGenerator::~CRelsGenerator()
{
}

void PPT_FORMAT::CRelsGenerator::Clear()
{
    m_oWriter.ClearNoAttack();
    m_lNextRelsID = 1;
    m_mapMediaRelsID.clear();
    m_mapHyperlinks.clear();
}

void PPT_FORMAT::CRelsGenerator::StartMaster(int nIndexTheme, int nStartLayoutIndex, int nCountLayouts)
{
    std::wstring str1 = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\
                           <Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">");

    m_oWriter.WriteString(str1);

    for (int i = 0; i < nCountLayouts; ++i)
    {
        std::wstring str = L"<Relationship Id=\"rId" + std::to_wstring(m_lNextRelsID++) + L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideLayout\" Target=\"../slideLayouts/slideLayout"
                + std::to_wstring(nStartLayoutIndex + 1) + L".xml\"/>";
        nStartLayoutIndex++;
        m_oWriter.WriteString(str);
    }

    std::wstring s = L"<Relationship Id=\"rId" + std::to_wstring(m_lNextRelsID++) + L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme\" Target=\"../theme/theme"
            + std::to_wstring(nIndexTheme + 1) + L".xml\"/>";
    m_oWriter.WriteString(s);
}

void PPT_FORMAT::CRelsGenerator::StartLayout(int nIndexTheme)
{
    std::wstring str1 = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\
                           <Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">");

    m_oWriter.WriteString(str1);

    std::wstring str = L"<Relationship Id=\"rId" + std::to_wstring(m_lNextRelsID++) +
            L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideMaster\" Target=\"../slideMasters/slideMaster"
            + std::to_wstring(nIndexTheme + 1) + L".xml\"/>";

    m_oWriter.WriteString(str);
}

void PPT_FORMAT::CRelsGenerator::StartNotes(int nIndexSlide, bool bMaster)
{
    m_oWriter.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\
                          <Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">");
    if (bMaster)
    {
        m_oWriter.WriteString(L"<Relationship Id=\"rId" + std::to_wstring(m_lNextRelsID++) +
                              L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/notesMaster\" Target=\"../notesMasters/notesMaster1.xml\"/>");
    }
    if (nIndexSlide >= 0)
    {
        m_oWriter.WriteString(L"<Relationship Id=\"rId" + std::to_wstring(m_lNextRelsID++) +
                              L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slide\" Target=\"../slides/slide"
                              + std::to_wstring(nIndexSlide + 1) + L".xml\"/>");
    }
}

void PPT_FORMAT::CRelsGenerator::StartSlide(int nIndexLayout, int nIndexNotes)
{
    m_oWriter.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\
                          <Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">");

    m_oWriter.WriteString(L"<Relationship Id=\"rId" + std::to_wstring(m_lNextRelsID++) + L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideLayout\" Target=\"../slideLayouts/slideLayout"
                          + std::to_wstring(nIndexLayout + 1) + L".xml\"/>");

    if (nIndexNotes >= 0)
    {
        m_oWriter.WriteString(L"<Relationship Id=\"rId" + std::to_wstring(m_lNextRelsID++) +
                              L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/notesSlide\" Target=\"../notesSlides/notesSlide"
                              + std::to_wstring(nIndexNotes + 1) + L".xml\"/>");
    }
}

void PPT_FORMAT::CRelsGenerator::CloseRels()
{
    std::wstring str = _T("</Relationships>");
    m_oWriter.WriteString(str);
}

void PPT_FORMAT::CRelsGenerator::SaveRels(const std::wstring &strFile)
{
    NSFile::CFileBinary oFile;
    oFile.CreateFileW(strFile);
    std::wstring strMem = m_oWriter.GetData();
    oFile.WriteStringUTF8(strMem);
    oFile.CloseFile();
}

std::wstring PPT_FORMAT::CRelsGenerator::WriteHyperlink(const std::wstring &strHyperlink, bool isExternal)
{
    std::map<std::wstring, std::wstring>::iterator pPair = m_mapHyperlinks.find(strHyperlink);

    if (m_mapHyperlinks.end() != pPair)
    {
        std::wstring strRid = L"rId" + pPair->second;
        return strRid;
    }
    m_mapHyperlinks[strHyperlink] = std::to_wstring(m_lNextRelsID);

    std::wstring strRid = L"rId" + std::to_wstring( m_lNextRelsID++);

    std::wstring strRels = L"<Relationship Id=\"";

    strRels += strRid + L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink\" Target=\"";
    strRels += XmlUtils::EncodeXmlStringExtend(strHyperlink);
    strRels += (isExternal ? L"\" TargetMode=\"External\" />" : L"\"/>");

    m_oWriter.WriteString(strRels);

    return strRid;
}

std::wstring PPT_FORMAT::CRelsGenerator::WriteFile(const std::wstring &strPath)
{
    std::map<std::wstring, std::wstring>::iterator pPair = m_mapHyperlinks.find(strPath);

    if (m_mapHyperlinks.end() != pPair)
    {
        std::wstring strRid = L"rId" + pPair->second;
        return strRid;
    }
    m_mapHyperlinks[strPath] = std::to_wstring( m_lNextRelsID );

    std::wstring strRid = L"rId" + std::to_wstring( m_lNextRelsID++);

    std::wstring strRels = L"<Relationship Id=\"";

    strRels += strRid + L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink\" Target=\"";
    strRels += L"file:///" + strPath + L"\" TargetMode=\"External\" />";

    m_oWriter.WriteString(strRels);

    return strRid;
}

std::wstring PPT_FORMAT::CRelsGenerator::WriteHyperlinkMedia(const std::wstring &strMedia, bool bExternal, bool newRIdAlways, std::wstring strRelsType)
{
    if (!newRIdAlways)
    {
        std::map<std::wstring, int>::iterator pPair = m_mapMediaRelsID.find(strMedia);

        if (m_mapMediaRelsID.end() != pPair)
        {
            std::wstring strRid = L"rId" + std::to_wstring(pPair->second);
            return strRid;
        }

        m_mapMediaRelsID[strMedia] = m_lNextRelsID;
    }

    std::wstring strRid	= L"rId" + std::to_wstring(m_lNextRelsID++);

    std::wstring strRels = L"<Relationship Id=\"" + strRid + L"\"" + L" Type=\"" + strRelsType + L"\""
            + L" Target=\"" + strMedia + L"\"";

    if (bExternal)
        strRels += L" TargetMode=\"External\"";
    strRels += L"/>";

    m_oWriter.WriteString(strRels);

    return strRid;
}

std::wstring PPT_FORMAT::CRelsGenerator::WriteHyperlinkImage(const std::wstring &strImage, bool bExternal)
{
    return WriteHyperlinkMedia(strImage, bExternal, false, L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image");
}

std::wstring PPT_FORMAT::CRelsGenerator::WriteHyperlinkAudio(const std::wstring &strImage, bool bExternal)
{
    return WriteHyperlinkMedia(strImage, bExternal, false, L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/audio");
}

std::wstring PPT_FORMAT::CRelsGenerator::WriteHyperlinkVideo(const std::wstring &strImage, bool bExternal)
{
    return WriteHyperlinkMedia(strImage, bExternal, false, L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/video");
}

std::wstring PPT_FORMAT::CRelsGenerator::WriteMedia(const std::wstring &strMediaPath)
{
    std::wstring strMedia = m_pManager->FindMedia(strMediaPath);

    if (strMedia.empty())	return WriteHyperlinkMedia(CorrectXmlString3(strMediaPath), true, true);
    return WriteHyperlinkMedia(strMedia, false, false); // changed
}

std::wstring PPT_FORMAT::CRelsGenerator::WriteImage(const std::wstring &strImagePath)
{
    std::wstring strImage = m_pManager->GenerateImage(strImagePath);

    if (strImage.empty())	return WriteHyperlinkImage(CorrectXmlString3(strImagePath), true);
    return WriteHyperlinkImage(strImage, false);
}

std::wstring PPT_FORMAT::CRelsGenerator::WriteSlideRef(const std::wstring &strLocation)
{
    int sldNum = CExFilesInfo::GetSlideNumber(strLocation);

    std::wstring strSlide(L"slide");
    strSlide += std::to_wstring(sldNum);
    strSlide += L".xml";
    return WriteHyperlinkMedia(strSlide, false, false, L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slide");
}

std::wstring PPT_FORMAT::CRelsGenerator::WriteAudio(const std::wstring &strAudioPath, bool &bExternal)
{
    std::wstring strAudio = m_pManager->GenerateAudio(strAudioPath);

    if (strAudio.empty())
    {
        bExternal = true;
        return WriteHyperlinkAudio(CorrectXmlString3(strAudioPath), true);
    }
    else
    {
        return WriteHyperlinkAudio(strAudio, false);
    }
}

std::wstring PPT_FORMAT::CRelsGenerator::WriteVideo(const std::wstring &strVideoPath, bool &bExternal)
{
    std::wstring strVideo = m_pManager->GenerateVideo(strVideoPath);

    if (strVideo.empty())
    {
        bExternal = true;
        return WriteHyperlinkVideo(CorrectXmlString3(strVideoPath), true);
    }
    else
    {
        return WriteHyperlinkVideo(strVideo, false);
    }
}
