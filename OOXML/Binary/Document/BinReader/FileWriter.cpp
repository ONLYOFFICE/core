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
	//Rels и ContentTypes пишем в конце
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

