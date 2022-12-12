/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
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

#include "fontTableWriter.h"

namespace Writers
{
	FontTableWriter::FontTableWriter(std::wstring sDir, std::wstring sFontDir, bool bNoFontDir) : m_sDir(sDir)
	{
		m_pApplicationFonts = NSFonts::NSApplication::Create();
		m_pFontManager = NULL;
		if(!bNoFontDir)
		{
			if(sFontDir.empty())
				m_pApplicationFonts->Initialize();
			else
				m_pApplicationFonts->InitializeFromFolder(sFontDir);
			m_pFontManager = m_pApplicationFonts->GenerateFontManager();
		}
	}
	FontTableWriter::~FontTableWriter()
	{
		RELEASEOBJECT(m_pFontManager);
		RELEASEOBJECT(m_pApplicationFonts);
	}

	void FontTableWriter::Write(bool bGlossary)
	{
		m_oWriter.WriteString(g_string_ft_Start);

		//Те шрифты которые всегда пишем в FontTable
		bool bCalibri = false;
		bool bTimes = false;
		bool bCambria = false;
		for (boost::unordered_map<std::wstring, char>::const_iterator it = m_mapFonts.begin(); it != m_mapFonts.end(); ++it)
		{
			const std::wstring& sFontName = it->first;
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

		OOX::CPath filePath = m_sDir + FILE_SEPARATOR_STR +_T("word") + (bGlossary ? (FILE_SEPARATOR_STR + std::wstring(L"glossary")) : L"") + FILE_SEPARATOR_STR + _T("fontTable.xml");

		NSFile::CFileBinary oFile;
		oFile.CreateFileW(filePath.GetPath());

		oFile.WriteStringUTF8(m_oWriter.GetData());
		oFile.CloseFile();
	}
	void FontTableWriter::WriteFont(std::wstring sFontName)
		{
			std::wstring sPanose;
			bool bUsePanose = false;
			if(NULL != m_pFontManager)
			{
				NSFonts::CFontSelectFormat oFontSelectFormat;
				oFontSelectFormat.wsName = new std::wstring;
				*oFontSelectFormat.wsName = sFontName;

				NSFonts::CFontInfo* pFontInfo = m_pFontManager->GetFontInfoByParams(oFontSelectFormat);
				if(NULL != pFontInfo)
				{
					for (size_t i = 0; i < 10; ++i)
					{
						BYTE cElem = pFontInfo->m_aPanose[i];
						if(0 != cElem)
							bUsePanose = true;
						sPanose += XmlUtils::ToString(cElem, L"%02X");
					}

				}
			}

			sFontName = XmlUtils::EncodeXmlString(sFontName);
			m_oWriter.WriteString(_T("<w:font w:name=\"") + sFontName + _T("\">"));
			if (bUsePanose && !sPanose.empty())
				m_oWriter.WriteString(_T("<w:panose1 w:val=\"")+sPanose+_T("\"/>"));
			m_oWriter.WriteString(std::wstring(_T("</w:font>")));
		}
}

