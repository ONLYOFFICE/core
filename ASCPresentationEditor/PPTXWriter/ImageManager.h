#pragma once

#ifndef DISABLE_FILE_DOWNLOADER
#include "../../Common/FileDownloader.h"
#endif

namespace NSPresentationEditor
{
	class CImageManager
	{
	private:
		std::map<std::wstring, std::wstring>	m_mapImages;

		LONG									m_lIndexNextImage;
		std::wstring							m_strDstMedia;

	public:
		CImageManager() : m_lIndexNextImage(0)
		{
		}
		~CImageManager()
		{
		}
		AVSINLINE void Clear()
		{
			m_mapImages.clear();
			m_lIndexNextImage = 0;
		}
		AVSINLINE void SetDstMedia(const CString& strDst)
		{
			m_strDstMedia = strDst;
		}

	public:
		AVSINLINE std::wstring GenerateImage(const std::wstring& strInput)
		{
			std::map<std::wstring, std::wstring>::iterator pPair = m_mapImages.find(strInput);
			if (m_mapImages.end() != pPair)
			{
				return pPair->second;
			}

			if (IsNeedDownload(strInput))
				return DownloadImage(strInput);

			std::wstring strExts = _T(".jpg");
			int nIndexExt = strInput.rfind(TCHAR('.'));
			if (-1 != nIndexExt)
				strExts = strInput.substr(nIndexExt);

			if (strExts == _T(".tmp"))	strExts = _T(".png");

			CString strImage = _T("");
			strImage.Format(_T("image%d"), m_lIndexNextImage++);

			std::wstring strOutput = m_strDstMedia + string2std_string(strImage) + strExts;		
			strImage  = _T("../media/") + strImage + std_string2string(strExts);

			// теперь нужно скопировать картинку
			if (strOutput != strInput)
			{
				if (CDirectory::CopyFile(std_string2string(strInput), std_string2string(strOutput), NULL, NULL) == false)
				{
					return L"";
				}
			}
			m_mapImages[strInput] = string2std_string(strImage);
			return strImage;
		}

		AVSINLINE bool IsNeedDownload(const std::wstring& strFile)
		{
			int n1 = strFile.find(_T("www"));
			int n2 = strFile.find(_T("http"));
			int n3 = strFile.find(_T("ftp"));

			if (((n1 >= 0) && (n1 < 10)) || ((n2 >= 0) && (n2 < 10)) || ((n3 >= 0) && (n3 < 10)))
				return true;
			return false;
		}
		AVSINLINE std::wstring DownloadImage(const std::wstring& strFile)
		{
#ifndef DISABLE_FILE_DOWNLOADER
            CFileDownloader oDownloader(strFile, TRUE);
			oDownloader.Start( 1 );
			while ( oDownloader.IsRunned() )
			{
				::Sleep( 10 );
			}

			if ( oDownloader.IsFileDownloaded() )
			{
				return GenerateImage( oDownloader.GetFilePath() );
			}
#endif
			return _T("");
		}
	};
	static std::wstring CorrectXmlString3(const std::wstring & str)
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
	class CRelsGenerator
	{
	private:
		NSPresentationEditor::CStringWriter		m_oWriter;
		int										m_lNextRelsID;
		std::map<std::wstring, int>				m_mapImages;
		CImageManager*							m_pManager;
		std::map<std::wstring, std::wstring>	m_mapHyperlinks;

	public:
		CRelsGenerator(CImageManager* pManager) : m_oWriter(), m_lNextRelsID(1)
		{
			m_pManager = pManager;
		}
		~CRelsGenerator()
		{
		}
		AVSINLINE void Clear()
		{
			m_oWriter.ClearNoAttack();
			m_lNextRelsID = 1;
			m_mapImages.clear();
			m_mapHyperlinks.clear();
		}

		AVSINLINE void StartMaster(int nIndexTheme, int nStartLayoutIndex, int nCountLayouts)
		{
			CString str1 = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\
<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">");

			m_oWriter.WriteString(str1);

			int nCurrent = nStartLayoutIndex;
			for (int i = 0; i < nCountLayouts; ++i)
			{
				CString str = _T("");
				str.Format(_T("<Relationship Id=\"rId%d\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideLayout\" Target=\"../slideLayouts/slideLayout%d.xml\"/>"), 
					m_lNextRelsID++, nCurrent + 1);
				++nCurrent;
				m_oWriter.WriteString(str);
			}

			CString s = _T("");
			s.Format(_T("<Relationship Id=\"rId%d\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme\" Target=\"../theme/theme%d.xml\"/>" ),
				m_lNextRelsID++, nIndexTheme + 1);
			m_oWriter.WriteString(s);
		}
		AVSINLINE void StartLayout(int nIndexTheme)
		{
			CString str1 = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\
<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">");

			m_oWriter.WriteString(str1);

			CString str = _T("");
			str.Format(_T("<Relationship Id=\"rId%d\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideMaster\" Target=\"../slideMasters/slideMaster%d.xml\"/>"), 
				m_lNextRelsID++, nIndexTheme + 1);
			m_oWriter.WriteString(str);
		}
		AVSINLINE void StartSlide(int nIndexLayout, int nIndexSlide)
		{
			CString str1 = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\
<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">");

			m_oWriter.WriteString(str1);

			CString str = _T("");
			str.Format(_T("<Relationship Id=\"rId%d\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideLayout\" Target=\"../slideLayouts/slideLayout%d.xml\"/>"), 
				m_lNextRelsID++, nIndexLayout + 1);
			m_oWriter.WriteString(str);

			str = _T("");
			str.Format(_T("<Relationship Id=\"rId%d\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/notesSlide\" Target=\"../notesSlides/notesSlide%d.xml\"/>"), m_lNextRelsID++, nIndexSlide + 1);
			m_oWriter.WriteString(str);
		}
		static AVSINLINE void StartNotes(int nIndexSlide, CString strDirectory, CString strComment)
		{
			CString sNum = _T("");
			sNum.Format(_T("%d"), nIndexSlide + 1);

			CString strNoteSlideRels = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\
<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">\
<Relationship Id=\"rId2\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slide\" Target=\"../slides/slide") + sNum + _T(".xml\"/>\
<Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/notesMaster\" Target=\"../notesMasters/notesMaster1.xml\"/>\
</Relationships>");

			CString strNoteRels = strDirectory	+ FILE_SEPARATOR_STR + _T("ppt") +
												+ FILE_SEPARATOR_STR + _T("notesSlides")
												+ FILE_SEPARATOR_STR + _T("_rels")
												+ FILE_SEPARATOR_STR + _T("notesSlide") + sNum + _T(".xml.rels");
			CFile oFile;
			oFile.CreateFile(strNoteRels);
			oFile.WriteStringUTF8(strNoteSlideRels);
			oFile.CloseFile();

			CString strNoteSlide = strDirectory + FILE_SEPARATOR_STR + _T("ppt") +
												+ FILE_SEPARATOR_STR + _T("notesSlides")
												+ FILE_SEPARATOR_STR + _T("notesSlide") + sNum + _T(".xml");
			oFile.CreateFile(strNoteSlide);

			CString strW1 = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\
<p:notes xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:p=\"http://schemas.openxmlformats.org/presentationml/2006/main\">\
<p:cSld><p:spTree><p:nvGrpSpPr><p:cNvPr id=\"1\" name=\"\"/><p:cNvGrpSpPr/><p:nvPr/></p:nvGrpSpPr><p:grpSpPr><a:xfrm><a:off x=\"0\" y=\"0\"/>\
<a:ext cx=\"0\" cy=\"0\"/><a:chOff x=\"0\" y=\"0\"/><a:chExt cx=\"0\" cy=\"0\"/></a:xfrm></p:grpSpPr><p:sp><p:nvSpPr><p:cNvPr id=\"100000\" name=\"\"/>\
<p:cNvSpPr><a:spLocks noGrp=\"1\" noChangeArrowheads=\"1\"/></p:cNvSpPr><p:nvPr><p:ph type=\"body\" idx=\"1\"/></p:nvPr></p:nvSpPr><p:spPr/>\
<p:txBody><a:bodyPr/><a:lstStyle/><a:p><a:r><a:rPr smtClean=\"0\"/><a:t>");
			CString strW2 = _T("</a:t></a:r><a:endParaRPr/></a:p></p:txBody></p:sp></p:spTree></p:cSld><p:clrMapOvr><a:masterClrMapping/></p:clrMapOvr></p:notes>");

			CorrectXmlString(strComment);

			oFile.WriteStringUTF8(strW1);
			oFile.WriteStringUTF8(strComment);
			oFile.WriteStringUTF8(strW2);

			oFile.CloseFile();
		}
		AVSINLINE void CloseRels()
		{
			CString str = _T("</Relationships>");
			m_oWriter.WriteString(str);
		}
		AVSINLINE void SaveRels(const CString& strFile)
		{
			CFile oFile;
			oFile.CreateFile(strFile);
			CString strMem = m_oWriter.GetData();
			oFile.WriteStringUTF8(strMem);
			oFile.CloseFile();
		}
		AVSINLINE CString WriteHyperlink(const std::wstring& strHyperlink)
		{
			std::map<std::wstring, std::wstring>::iterator pPair = m_mapHyperlinks.find(strHyperlink);

			if (m_mapHyperlinks.end() != pPair)
			{
				CString strRid = _T("");
				strRid.Format(_T("rId%d"), pPair->second);
				return strRid;
			}
			m_mapHyperlinks[strHyperlink] = m_lNextRelsID;

			CString strRid = _T("");
			strRid.Format(_T("rId%d"), m_lNextRelsID++);

			std::wstring strRels = _T("<Relationship Id=\"") ;

            strRels += string2std_string(strRid) + _T("\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink\" Target=\"");
            strRels += strHyperlink + _T("\"/>");

			m_oWriter.WriteString(strRels);
		}

		AVSINLINE CString WriteHyperlinkImage(const std::wstring& strImage, bool bExternal = true)
		{
			std::map<std::wstring, int>::iterator pPair = m_mapImages.find(strImage);

			if (m_mapImages.end() != pPair)
			{
				CString strRid = _T("");
				strRid.Format(_T("rId%d"), pPair->second);
				return strRid;
			}

			m_mapImages[strImage] = m_lNextRelsID;

			CString strRid = _T("");
			strRid.Format(_T("rId%d"), m_lNextRelsID++);

			std::wstring strRels = _T("<Relationship Id=\"") ;

            strRels += string2std_string(strRid) + _T("\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image\" Target=\"");
            strRels += strImage + _T("\"");

			if (bExternal)
				strRels += std::wstring(L" TargetMode=\"External\"");
			strRels += std::wstring(L"/>");

			m_oWriter.WriteString(strRels);

			return strRid;
		}
		AVSINLINE CString WriteImage(const std::wstring& strImagePath)
		{
			std::wstring strImage = m_pManager->GenerateImage(strImagePath);

			if (strImage.empty()) return WriteHyperlinkImage(CorrectXmlString3(strImagePath), true);			
			return WriteHyperlinkImage(strImage, false);
		}
		
	};
}
