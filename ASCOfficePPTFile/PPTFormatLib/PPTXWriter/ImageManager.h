/*
 * (c) Copyright Ascensio System SIA 2010-2016
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#pragma once

#ifndef DISABLE_FILE_DOWNLOADER
#include "../../../Common/FileDownloader/FileDownloader.h"
#endif

namespace NSPresentationEditor
{
	class CImageManager
	{
	private:
		std::map<std::wstring, std::wstring>	m_mapImages;

        long									m_lIndexNextImage;
		std::wstring							m_strDstMedia;

	public:
		CImageManager() : m_lIndexNextImage(0)
		{
		}
		~CImageManager()
		{
		}
        inline void Clear()
		{
			m_mapImages.clear();
			m_lIndexNextImage = 0;
		}
        inline void SetDstMedia(const std::wstring& strDst)
		{
			m_strDstMedia = strDst;
		}

	public:
        inline std::wstring GenerateImage(const std::wstring& strInput)
		{
			std::map<std::wstring, std::wstring>::iterator pPair = m_mapImages.find(strInput);
			if (m_mapImages.end() != pPair)
			{
				return pPair->second;
			}

			if (IsNeedDownload(strInput))
				return DownloadImage(strInput);

			std::wstring strExts = _T(".jpg");
            int nIndexExt = strInput.rfind(wchar_t('.'));
			if (-1 != nIndexExt)
				strExts = strInput.substr(nIndexExt);

			if (strExts == _T(".tmp"))	strExts = _T(".png");

			std::wstring strImage = L"image" + std::to_wstring(m_lIndexNextImage++);

			std::wstring strOutput = m_strDstMedia + strImage + strExts;		
			strImage  = _T("../media/") + strImage + strExts;

			// теперь нужно скопировать картинку
			if (strOutput != strInput)
			{
                if (CDirectory::CopyFile(strInput, strOutput) == false)
				{
					return L"";
				}
			}
			m_mapImages[strInput] = strImage;
			return strImage;
		}

        inline bool IsNeedDownload(const std::wstring& strFile)
		{
			int n1 = strFile.find(L"www");
			int n2 = strFile.find(L"http");
			int n3 = strFile.find(L"ftp");

			if (((n1 >= 0) && (n1 < 10)) || ((n2 >= 0) && (n2 < 10)) || ((n3 >= 0) && (n3 < 10)))
				return true;
			return false;
		}
        inline std::wstring DownloadImage(const std::wstring& strFile)
		{
#ifndef DISABLE_FILE_DOWNLOADER
            CFileDownloader oDownloader(strFile, TRUE);
			if ( oDownloader.DownloadSync() )
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
        inline void Clear()
		{
			m_oWriter.ClearNoAttack();
			m_lNextRelsID = 1;
			m_mapImages.clear();
			m_mapHyperlinks.clear();
		}

        inline void StartMaster(int nIndexTheme, int nStartLayoutIndex, int nCountLayouts)
		{
            std::wstring str1 = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\
<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">");

			m_oWriter.WriteString(str1);

			int nCurrent = nStartLayoutIndex;
			for (int i = 0; i < nCountLayouts; ++i)
			{
                std::wstring str = L"<Relationship Id=\"rId" + std::to_wstring(m_lNextRelsID++) + L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideLayout\" Target=\"../slideLayouts/slideLayout"
                        + std::to_wstring(nCurrent + 1) + L".xml\"/>";
				++nCurrent;
				m_oWriter.WriteString(str);
			}

            std::wstring s = L"<Relationship Id=\"rId" + std::to_wstring(m_lNextRelsID++) + L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme\" Target=\"../theme/theme"
                    + std::to_wstring(nIndexTheme + 1) + L".xml\"/>";
			m_oWriter.WriteString(s);
		}
        inline void StartLayout(int nIndexTheme)
		{
            std::wstring str1 = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\
<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">");

			m_oWriter.WriteString(str1);

            std::wstring str = L"<Relationship Id=\"rId" + std::to_wstring(m_lNextRelsID++) +
                    L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideMaster\" Target=\"../slideMasters/slideMaster"
                    + std::to_wstring(nIndexTheme + 1) + L".xml\"/>";

			m_oWriter.WriteString(str);
		}
        inline void StartSlide(int nIndexLayout, int nIndexSlide)
		{
            m_oWriter.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\
                                  <Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">");

            m_oWriter.WriteString(L"<Relationship Id=\"rId" + std::to_wstring(m_lNextRelsID++) + L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideLayout\" Target=\"../slideLayouts/slideLayout"
                                  + std::to_wstring(nIndexLayout + 1) + L".xml\"/>");

            m_oWriter.WriteString(L"<Relationship Id=\"rId" + std::to_wstring(m_lNextRelsID++) +
                                  L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/notesSlide\" Target=\"../notesSlides/notesSlide"
                                  + std::to_wstring(nIndexSlide + 1) + L".xml\"/>");
		}
        static inline void StartNotes(int nIndexSlide, std::wstring strDirectory, std::wstring strComment)
		{
            std::wstring sNum = std::to_wstring( nIndexSlide + 1);

            std::wstring strNoteSlideRels = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\
<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">\
<Relationship Id=\"rId2\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slide\" Target=\"../slides/slide") + sNum + _T(".xml\"/>\
<Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/notesMaster\" Target=\"../notesMasters/notesMaster1.xml\"/>\
</Relationships>");

            std::wstring strNoteRels = strDirectory	+ FILE_SEPARATOR_STR + _T("ppt") +
												+ FILE_SEPARATOR_STR + _T("notesSlides")
												+ FILE_SEPARATOR_STR + _T("_rels")
												+ FILE_SEPARATOR_STR + _T("notesSlide") + sNum + _T(".xml.rels");
			CFile oFile;
			oFile.CreateFile(strNoteRels);
			oFile.WriteStringUTF8(strNoteSlideRels);
			oFile.CloseFile();

            std::wstring strNoteSlide = strDirectory + FILE_SEPARATOR_STR + _T("ppt") +
												+ FILE_SEPARATOR_STR + _T("notesSlides")
												+ FILE_SEPARATOR_STR + _T("notesSlide") + sNum + _T(".xml");
			oFile.CreateFile(strNoteSlide);

            std::wstring strW1 = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\
<p:notes xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:p=\"http://schemas.openxmlformats.org/presentationml/2006/main\">\
<p:cSld><p:spTree><p:nvGrpSpPr><p:cNvPr id=\"1\" name=\"\"/><p:cNvGrpSpPr/><p:nvPr/></p:nvGrpSpPr><p:grpSpPr><a:xfrm><a:off x=\"0\" y=\"0\"/>\
<a:ext cx=\"0\" cy=\"0\"/><a:chOff x=\"0\" y=\"0\"/><a:chExt cx=\"0\" cy=\"0\"/></a:xfrm></p:grpSpPr><p:sp><p:nvSpPr><p:cNvPr id=\"100000\" name=\"\"/>\
<p:cNvSpPr><a:spLocks noGrp=\"1\" noChangeArrowheads=\"1\"/></p:cNvSpPr><p:nvPr><p:ph type=\"body\" idx=\"1\"/></p:nvPr></p:nvSpPr><p:spPr/>\
<p:txBody><a:bodyPr/><a:lstStyle/><a:p><a:r><a:rPr smtClean=\"0\"/><a:t>");
            std::wstring strW2 = _T("</a:t></a:r><a:endParaRPr/></a:p></p:txBody></p:sp></p:spTree></p:cSld><p:clrMapOvr><a:masterClrMapping/></p:clrMapOvr></p:notes>");

			CorrectXmlString(strComment);

			oFile.WriteStringUTF8(strW1);
			oFile.WriteStringUTF8(strComment);
			oFile.WriteStringUTF8(strW2);

			oFile.CloseFile();
		}
        inline void CloseRels()
		{
            std::wstring str = _T("</Relationships>");
			m_oWriter.WriteString(str);
		}
        inline void SaveRels(const std::wstring& strFile)
		{
			CFile oFile;
			oFile.CreateFile(strFile);
            std::wstring strMem = m_oWriter.GetData();
			oFile.WriteStringUTF8(strMem);
			oFile.CloseFile();
		}
        inline std::wstring WriteHyperlink(const std::wstring& strHyperlink)
		{
			std::map<std::wstring, std::wstring>::iterator pPair = m_mapHyperlinks.find(strHyperlink);

			if (m_mapHyperlinks.end() != pPair)
			{
                std::wstring strRid = L"rId" + pPair->second;
				return strRid;
			}
			m_mapHyperlinks[strHyperlink] = m_lNextRelsID;

            std::wstring strRid = L"rId" + std::to_wstring( m_lNextRelsID++);

			std::wstring strRels = L"<Relationship Id=\"";

            strRels += strRid + L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink\" Target=\"";
            strRels += strHyperlink + L"\"/>";

			m_oWriter.WriteString(strRels);
		
            return strRid;
        }

        inline std::wstring WriteHyperlinkImage(const std::wstring& strImage, bool bExternal = true)
		{
			std::map<std::wstring, int>::iterator pPair = m_mapImages.find(strImage);

			if (m_mapImages.end() != pPair)
			{
                std::wstring strRid = L"rId" + std::to_wstring(pPair->second);
				return strRid;
			}

			m_mapImages[strImage] = m_lNextRelsID;

			std::wstring strRid		= L"rId" + std::to_wstring(m_lNextRelsID++);
			
			std::wstring strRels	= L"<Relationship Id=\"" + strRid + L"\"";
            strRels += L" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image\"";
            strRels += L" Target=\"" + strImage + L"\"";

			if (bExternal)
				strRels += L" TargetMode=\"External\"";
			strRels += L"/>";

			m_oWriter.WriteString(strRels);

			return strRid;
		}
        inline std::wstring WriteImage(const std::wstring& strImagePath)
		{
			std::wstring strImage = m_pManager->GenerateImage(strImagePath);

			if (strImage.empty()) return WriteHyperlinkImage(CorrectXmlString3(strImagePath), true);			
			return WriteHyperlinkImage(strImage, false);
		}
		
	};
}
