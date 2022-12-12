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
