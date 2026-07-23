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

#include "OOXColorReader.h"
#include "OOXColorSchemeReader.h"

OOXColorSchemeReader::OOXColorSchemeReader(PPTX::nsTheme::ClrScheme * ooxColorScheme)
{
	m_ooxColorScheme = ooxColorScheme;
}
bool OOXColorSchemeReader::Parse( ReaderParameter oParam )
{
	if (m_ooxColorScheme == NULL) return false;

	RtfColor		oNewColor;
	OOXColorReader	oColorReader(m_ooxColorScheme);

	oColorReader.Parse( oParam, L"accent1", oNewColor);
	oNewColor.m_eTheme = RtfColor::caccentone;
	oParam.oRtf->m_oColorTable.AddItem( oNewColor );

	oColorReader.Parse( oParam, L"accent2", oNewColor);
	oNewColor.m_eTheme = RtfColor::caccenttwo;
	oParam.oRtf->m_oColorTable.AddItem( oNewColor );

	oColorReader.Parse( oParam, L"accent3", oNewColor);
	oNewColor.m_eTheme = RtfColor::caccentthree;
	oParam.oRtf->m_oColorTable.AddItem( oNewColor );

	oColorReader.Parse( oParam, L"accent4", oNewColor);
	oNewColor.m_eTheme = RtfColor::caccentfour;
	oParam.oRtf->m_oColorTable.AddItem( oNewColor );

	oColorReader.Parse( oParam, L"accent5", oNewColor);
	oNewColor.m_eTheme = RtfColor::caccentfive;
	oParam.oRtf->m_oColorTable.AddItem( oNewColor );

	oColorReader.Parse( oParam, L"accent6", oNewColor);
	oNewColor.m_eTheme = RtfColor::caccentsix;
	oParam.oRtf->m_oColorTable.AddItem( oNewColor );

	oColorReader.Parse( oParam, L"dk1", oNewColor);
	oNewColor.m_eTheme = RtfColor::cmaindarkone;
	oParam.oRtf->m_oColorTable.AddItem( oNewColor );

	oColorReader.Parse( oParam, L"dk2", oNewColor);
	oNewColor.m_eTheme = RtfColor::cmaindarktwo;
	oParam.oRtf->m_oColorTable.AddItem( oNewColor );

	oColorReader.Parse( oParam, L"lt1", oNewColor);
	oNewColor.m_eTheme = RtfColor::cmainlightone;
	oParam.oRtf->m_oColorTable.AddItem( oNewColor );

	oColorReader.Parse( oParam, L"lt2", oNewColor);
	oNewColor.m_eTheme = RtfColor::cmainlighttwo;
	oParam.oRtf->m_oColorTable.AddItem( oNewColor );

	oColorReader.Parse( oParam, L"hlink", oNewColor);
	oNewColor.m_eTheme = RtfColor::chyperlink;
	oParam.oRtf->m_oColorTable.AddItem( oNewColor );

	oColorReader.Parse( oParam, L"folHlink", oNewColor);
	oNewColor.m_eTheme = RtfColor::cfollowedhyperlink;
	oParam.oRtf->m_oColorTable.AddItem( oNewColor );

	//else if( L"a:bg1" == sNodeName )
	//	oNewColor.m_eTheme = RtfColor::cbackgroundone;
	//else if( L"a:bg2" == sNodeName )
	//	oNewColor.m_eTheme = RtfColor::cbackgroundtwo;
	//else if( L"a:phClr" == sNodeName )
	//	oNewColor.m_eTheme = RtfColor::cmainlighttwo;
	//else if( L"a:tx1" == sNodeName )
	//	oNewColor.m_eTheme = RtfColor::ctextone;
	//else if( L"a:tx2" == sNodeName )
	//	oNewColor.m_eTheme = RtfColor::ctexttwo;


	return true;
}
