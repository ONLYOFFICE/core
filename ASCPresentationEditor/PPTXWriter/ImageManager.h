#pragma once
#include "../stdafx.h"
#include "FileDownloader.h"


namespace NSPresentationEditor
{
	class CImageManager
	{
	private:
		std::map<CString, CString>	m_mapImages;
		LONG						m_lIndexNextImage;
		CString						m_strDstMedia;

	public:
		CImageManager() : m_mapImages(), m_lIndexNextImage(0)
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
		AVSINLINE CString GenerateImage(const CString& strInput)
		{
			std::map<CString, CString>::iterator pPair = m_mapImages.find(strInput);
			if (m_mapImages.end() != pPair)
				return pPair->second;

			if (IsNeedDownload(strInput))
				return DownloadImage(strInput);

			CString strExts = _T(".jpg");
			int nIndexExt = strInput.ReverseFind(TCHAR('.'));
			if (-1 != nIndexExt)
				strExts = strInput.Mid(nIndexExt);

			if (strExts == _T(".tmp"))
				strExts = _T(".png");

			CString strImage = _T("");
			strImage.Format(_T("image%d"), m_lIndexNextImage++);

			CString strOutput = m_strDstMedia + strImage + strExts;		
			strImage  = _T("../media/") + strImage + strExts;
			m_mapImages[strInput] = strImage;

			// теперь нужно скопировать картинку
			if (strOutput != strInput)
				CDirectory::CopyFile(strInput, strOutput, NULL, NULL);
			return strImage;
		}

		AVSINLINE bool IsNeedDownload(const CString& strFile)
		{
			int n1 = strFile.Find(_T("www"));
			int n2 = strFile.Find(_T("http"));
			int n3 = strFile.Find(_T("ftp"));

			if (((n1 >= 0) && (n1 < 10)) || ((n2 >= 0) && (n2 < 10)) || ((n3 >= 0) && (n3 < 10)))
				return true;
			return false;
		}
		AVSINLINE CString DownloadImage(const CString& strFile)
		{
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
			return _T("");
		}
	};

	class CRelsGenerator
	{
	private:
		NSPresentationEditor::CStringWriter m_oWriter;
		int									m_lNextRelsID;
		std::map<CString, int>				m_mapImages;
		CImageManager*						m_pManager;

	public:
		CRelsGenerator(CImageManager* pManager) : m_oWriter(), m_lNextRelsID(1), m_mapImages()
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

			CString strNoteRels = strDirectory + _T("\\ppt\\notesSlides\\_rels\\notesSlide") + sNum + _T(".xml.rels");
			CFile oFile;
			oFile.CreateFile(strNoteRels);
			oFile.WriteStringUTF8(strNoteSlideRels);
			oFile.CloseFile();

			CString strNoteSlide = strDirectory + _T("\\ppt\\notesSlides\\notesSlide") + sNum + _T(".xml");
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

		AVSINLINE CString WriteImage(const CString& strImagePath)
		{
			CString strImage = m_pManager->GenerateImage(strImagePath);
			std::map<CString, int>::iterator pPair = m_mapImages.find(strImage);

			if (m_mapImages.end() != pPair)
			{
				CString strRid = _T("");
				strRid.Format(_T("rId%d"), pPair->second);
				return strRid;
			}

			m_mapImages[strImage] = m_lNextRelsID;

			CString strRid = _T("");
			strRid.Format(_T("rId%d"), m_lNextRelsID++);

			CString strRels = _T("");
			strRels.Format(_T("<Relationship Id=\"%s\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image\" Target=\"%s\"/>"),
				strRid, strImage);
			m_oWriter.WriteString(strRels);

			return strRid;
		}
		
	};
}