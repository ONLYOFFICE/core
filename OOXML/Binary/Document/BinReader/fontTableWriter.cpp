/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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

		//Fonts that are always written to FontTable
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

