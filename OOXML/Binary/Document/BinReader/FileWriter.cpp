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

#include "FileWriter.h"

#include "ReaderClasses.h"
#include "../../../DocxFormat/Core.h"
#include "../../../DocxFormat/App.h"

namespace Writers
{

FileWriter::FileWriter(std::wstring sDirOutput,std::wstring sFontDir, bool bNoFontDir, int nVersion, NSBinPptxRW::CDrawingConverter* pDrawingConverter, std::wstring sThemePath)
    :
	m_oMain(sDirOutput, sFontDir, bNoFontDir, nVersion),
	m_oGlossary(sDirOutput, sFontDir, bNoFontDir, nVersion),
	m_oMediaWriter		(sDirOutput),
	m_oChartWriter		(sDirOutput),
	m_oCustomXmlWriter	(sDirOutput, pDrawingConverter),
	m_pDrawingConverter	(pDrawingConverter),
	m_sThemePath		(sThemePath),
	m_oDocumentRelsWriter		(sDirOutput),
	m_nDocPrIndex		(0),
	m_pComments			(NULL),
	m_pApp				(NULL),
	m_pCore				(NULL),
	m_pCustomProperties	(NULL),
	m_pCurrentSettings	(NULL)
{
}
FileWriter::~FileWriter()
{
    RELEASEOBJECT(m_pApp);
    RELEASEOBJECT(m_pCore);
	RELEASEOBJECT(m_pCustomProperties);

	m_pCurrentSettings = NULL;
}
void FileWriter::Write()
{
	m_oMain.Write();
    m_oChartWriter.Write();
	//Write Rels and ContentTypes at the end
	m_oDocumentRelsWriter.Write();
}
void FileWriter::WriteGlossary()
{
	m_oGlossary.Write(true);
}
int FileWriter::getNextDocPr()
{
	m_nDocPrIndex++;
	return m_nDocPrIndex;
}
void FileWriter::AddSetting(std::wstring sSetting)
{
	if (m_bGlossaryMode) m_oGlossary.settings.AddSetting(sSetting);
	else m_oMain.settings.AddSetting(sSetting);
}
bool FileWriter::IsEmptyGlossary()
{
	return m_oGlossary.document.m_oContent.GetSize() < 1;
}
}

