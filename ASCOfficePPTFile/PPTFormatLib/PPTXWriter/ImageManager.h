﻿/*
 * (c) Copyright Ascensio System SIA 2010-2018
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
	class CMediaManager
	{
	private:
		std::map<std::wstring, std::wstring>	m_mapMedia;

		long									m_lIndexNextAudio;
		long									m_lIndexNextVideo;
		long									m_lIndexNextImage;

		std::wstring							m_strDstMedia;

	public:
		CMediaManager() : m_lIndexNextImage(0), m_lIndexNextAudio(0), m_lIndexNextVideo(0)
		{
		}
		~CMediaManager()
		{
		}
        inline void Clear()
		{
			m_mapMedia.clear();

			m_lIndexNextImage = 0;
			m_lIndexNextAudio = 0;
			m_lIndexNextVideo = 0;
		}
		inline std::wstring FindMedia(const std::wstring& strInput)
		{
			std::map<std::wstring, std::wstring>::iterator pPair = m_mapMedia.find(strInput);
			if (m_mapMedia.end() != pPair)
			{
				return pPair->second;
			}
			return L"";
		}
        inline void SetDstMedia(const std::wstring& strDst)
		{
			m_strDstMedia = strDst;
		}
		inline std::wstring GenerateVideo(const std::wstring& strInput)
		{
			return GenerateMedia(strInput, L"video", m_lIndexNextVideo, L".avi");
		}
		inline std::wstring GenerateAudio(const std::wstring& strInput)
		{
			return GenerateMedia(strInput, L"audio", m_lIndexNextAudio, L".wav");
		}
		inline std::wstring GenerateImage(const std::wstring& strInput)
		{
			return GenerateMedia(strInput, L"image", m_lIndexNextImage, L".png");
		}        
		inline std::wstring GenerateMedia(const std::wstring& strInput, const std::wstring& Template, long & Indexer, const std::wstring& strDefaultExt)
		{
			std::map<std::wstring, std::wstring>::iterator pPair = m_mapMedia.find(strInput);
			if (m_mapMedia.end() != pPair)
			{
				return pPair->second;
			}

			if (IsNeedDownload(strInput))
			{
#ifndef DISABLE_FILE_DOWNLOADER
				CFileDownloader oDownloader(strInput, TRUE);
				if ( oDownloader.DownloadSync() )
				{
					return GenerateMedia( oDownloader.GetFilePath(), Template, Indexer, strDefaultExt);
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
			if (strExts == _T(".tmp"))		strExts = strDefaultExt;

			std::wstring strMediaName = Template + std::to_wstring(++Indexer);

			std::wstring strOutput = m_strDstMedia + strMediaName + strExts;		
			strMediaName  = _T("../media/") + strMediaName + strExts;

			// теперь нужно скопировать
			if (strOutput != strInput)
			{
                if (CDirectory::CopyFile(strInput, strOutput) == false)
				{
					return L"";
				}
			}
			m_mapMedia[strInput] = strMediaName;
			return strMediaName;
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
		std::map<std::wstring, int>				m_mapMediaRelsID;
		CMediaManager*							m_pManager;
		std::map<std::wstring, std::wstring>	m_mapHyperlinks;

	public:
		CRelsGenerator(CMediaManager* pManager) : m_oWriter(), m_lNextRelsID(1)
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
			m_mapMediaRelsID.clear();
			m_mapHyperlinks.clear();
		}

        inline void StartMaster(int nIndexTheme, int nStartLayoutIndex, int nCountLayouts)
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
         inline void StartNotes(int nIndexSlide, bool bMaster)
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
		 inline void StartSlide(int nIndexLayout, int nIndexNotes)
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

		inline std::wstring WriteHyperlinkMedia(const std::wstring& strMedia, bool bExternal = true, bool newRIdAlways = false, std::wstring strRelsType = L"http://schemas.microsoft.com/office/2007/relationships/media")
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
        inline std::wstring WriteHyperlinkImage(const std::wstring& strImage, bool bExternal = true)
		{
			return WriteHyperlinkMedia(strImage, bExternal, false, L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image");
		}
        inline std::wstring WriteHyperlinkAudio(const std::wstring& strImage, bool bExternal = true)
		{
			return WriteHyperlinkMedia(strImage, bExternal, false, L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/audio");
		}
        inline std::wstring WriteHyperlinkVideo(const std::wstring& strImage, bool bExternal = true)
		{
			return WriteHyperlinkMedia(strImage, bExternal, false, L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/video");
		}
		inline std::wstring WriteMedia(const std::wstring& strMediaPath)
		{
			std::wstring strMedia = m_pManager->FindMedia(strMediaPath);

			if (strMedia.empty())	return WriteHyperlinkMedia(CorrectXmlString3(strMediaPath), true, true);			
									return WriteHyperlinkMedia(strMedia, false, true);
		}
		inline std::wstring WriteImage(const std::wstring& strImagePath)
		{
			std::wstring strImage = m_pManager->GenerateImage(strImagePath);

			if (strImage.empty())	return WriteHyperlinkImage(CorrectXmlString3(strImagePath), true);			
									return WriteHyperlinkImage(strImage, false);
		}
        inline std::wstring WriteAudio(const std::wstring& strAudioPath, bool & bExternal)
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
        inline std::wstring WriteVideo(const std::wstring& strVideoPath, bool & bExternal)
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
	};
}
