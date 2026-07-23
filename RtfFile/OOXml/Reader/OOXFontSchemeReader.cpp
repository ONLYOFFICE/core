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

#include "OOXFontSchemeReader.h"

OOXFontSchemeReader::OOXFontSchemeReader(PPTX::nsTheme::FontScheme * ooxFontScheme)
{
	m_ooxFontScheme = ooxFontScheme;
}
bool OOXFontSchemeReader::Parse( ReaderParameter oParam )
{
	if (m_ooxFontScheme == NULL) return false;

	oParam.oReader->m_smajorAscii = m_ooxFontScheme->majorFont.latin.typeface;
	oParam.oReader->m_smajorHAnsi = m_ooxFontScheme->majorFont.latin.typeface;

	oParam.oReader->m_smajorEastAsia	= m_ooxFontScheme->majorFont.ea.typeface;
	oParam.oReader->m_smajorBidi		= m_ooxFontScheme->majorFont.cs.typeface;

	oParam.oReader->m_sminorAscii = m_ooxFontScheme->minorFont.latin.typeface;
	oParam.oReader->m_sminorHAnsi = m_ooxFontScheme->minorFont.latin.typeface;

	oParam.oReader->m_sminorEastAsia = m_ooxFontScheme->minorFont.ea.typeface;
	oParam.oReader->m_sminorBidi = m_ooxFontScheme->minorFont.cs.typeface;
	//for (long i=0 ; i < m_ooxFontScheme->m_oMinorFont.m_arrFont.size(); i++)
	//{
	//}

	return true;
}
