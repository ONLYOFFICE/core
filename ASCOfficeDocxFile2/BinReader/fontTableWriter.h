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
#ifndef FONT_TABLE_WRITER
#define FONT_TABLE_WRITER

#include "../../XlsxSerializerCom/Common/Common.h"
#include "../../DesktopEditor/fontengine/FontManager.h"

namespace Writers
{
	static CString g_string_ft_Start = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:fonts xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" mc:Ignorable=\"w14\">");
	static CString g_string_ft_End = _T("</w:fonts>");

	class FontTableWriter
	{
		XmlUtils::CStringWriter	m_oWriter;
		CString	m_sDir;
		CApplicationFonts m_oApplicationFonts;
		CFontManager* m_pFontManager;
	public:
		std::map<CString, int> m_mapFonts;
	public:
		FontTableWriter(CString sDir, CString sFontDir):m_sDir(sDir)
		{
			if(sFontDir.IsEmpty())
				m_oApplicationFonts.Initialize();
			else
				m_oApplicationFonts.InitializeFromFolder(string2std_string(sFontDir));
			CFontList* pFontList = m_oApplicationFonts.GetList();
			if(NULL != pFontList)
			{
				std::wstring sDefaultFont(_T("Arial"));
				pFontList->SetDefaultFont(sDefaultFont);
			}
			m_pFontManager = m_oApplicationFonts.GenerateFontManager();
		}
		~FontTableWriter()
		{
			RELEASEOBJECT(m_pFontManager);
		}

		void Write()
		{
			m_oWriter.WriteString(g_string_ft_Start);

			//Те шрифты которые всегда пишем в FontTable
			bool bCalibri = false;
			bool bTimes = false;
			bool bCambria = false;
			for (std::map<CString, int>::const_iterator it = m_mapFonts.begin(); it != m_mapFonts.end(); ++it)
			{
				const CString& sFontName = it->first;
				if(_T("Calibri") == sFontName)
					bCalibri = true;
				else if(_T("Times New Roman") == sFontName)
					bTimes = true;
				else if(_T("Cambria") == sFontName)
					bCambria = true;
				WriteFont(sFontName);
			}
			if(false == bCalibri)
				WriteFont(_T("Calibri"));
			if(false == bTimes)
				WriteFont(_T("Times New Roman"));
			if(false == bCambria)
				WriteFont(_T("Cambria"));

			m_oWriter.WriteString(g_string_ft_End);

            OOX::CPath filePath = m_sDir + FILE_SEPARATOR_STR +_T("word") + FILE_SEPARATOR_STR + _T("fontTable.xml");

			CFile oFile;
			oFile.CreateFile(filePath.GetPath());

			oFile.WriteStringUTF8(m_oWriter.GetData());
			oFile.CloseFile();
		}
		void WriteFont(CString sFontName)
		{
			CString sPanose;
			bool bUsePanose = false;
			if(NULL != m_pFontManager)
			{
				CFontSelectFormat oFontSelectFormat;
				oFontSelectFormat.wsName = new std::wstring;
				*oFontSelectFormat.wsName = std::wstring(sFontName.GetString());
				CFontInfo* pFontInfo = m_pFontManager->GetFontInfoByParams(oFontSelectFormat);
				if(NULL != pFontInfo)
				{
					for(int i = 0; i < 10; ++i)
					{
						BYTE cElem = pFontInfo->m_aPanose[i];
						if(0 != cElem)
							bUsePanose = true;
						if(cElem > 0xF)
							sPanose.AppendFormat(_T("%X"), cElem);
						else
							sPanose.AppendFormat(_T("0%X"), cElem);
					}
					
				}
			}

			sFontName = XmlUtils::EncodeXmlString(sFontName);
			m_oWriter.WriteString(_T("<w:font w:name=\"") + sFontName + _T("\">"));
			if(bUsePanose && !sPanose.IsEmpty())
				m_oWriter.WriteString(_T("<w:panose1 w:val=\"")+sPanose+_T("\"/>"));
			m_oWriter.WriteString(CString(_T("</w:font>")));
		}
	};
}
#endif	// #ifndef FONT_TABLE_WRITER
