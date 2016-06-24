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
#include "OOXColorReader.h"
#include "../RtfProperty.h"

class OOXColorSchemeReader
{
private:
	OOX::Drawing::CColorScheme * m_ooxColorScheme;
public: 
	OOXColorSchemeReader(OOX::Drawing::CColorScheme * ooxColorScheme)
	{
		m_ooxColorScheme = ooxColorScheme;
	}

	bool Parse( ReaderParameter oParam )
	{
		if (m_ooxColorScheme == NULL) return false;

		RtfColor		oNewColor; 
		OOXColorReader	oColorReader;

		oColorReader.Parse( oParam, m_ooxColorScheme->m_oAccent1, oNewColor);
			oNewColor.m_eTheme = RtfColor::caccentone;
			oParam.oRtf->m_oColorTable.AddItem( oNewColor );

		oColorReader.Parse( oParam, m_ooxColorScheme->m_oAccent2, oNewColor);
			oNewColor.m_eTheme = RtfColor::caccenttwo;
			oParam.oRtf->m_oColorTable.AddItem( oNewColor );

		oColorReader.Parse( oParam, m_ooxColorScheme->m_oAccent3, oNewColor);
			oNewColor.m_eTheme = RtfColor::caccentthree;
			oParam.oRtf->m_oColorTable.AddItem( oNewColor );
		
		oColorReader.Parse( oParam, m_ooxColorScheme->m_oAccent4, oNewColor);
			oNewColor.m_eTheme = RtfColor::caccentfour;
			oParam.oRtf->m_oColorTable.AddItem( oNewColor );

		oColorReader.Parse( oParam, m_ooxColorScheme->m_oAccent5, oNewColor);
			oNewColor.m_eTheme = RtfColor::caccentfive;
			oParam.oRtf->m_oColorTable.AddItem( oNewColor );

		oColorReader.Parse( oParam, m_ooxColorScheme->m_oAccent6, oNewColor);
			oNewColor.m_eTheme = RtfColor::caccentsix;
			oParam.oRtf->m_oColorTable.AddItem( oNewColor );

		oColorReader.Parse( oParam, m_ooxColorScheme->m_oDk1, oNewColor);
			oNewColor.m_eTheme = RtfColor::cmaindarkone;
			oParam.oRtf->m_oColorTable.AddItem( oNewColor );
		
		oColorReader.Parse( oParam, m_ooxColorScheme->m_oDk2, oNewColor);
			oNewColor.m_eTheme = RtfColor::cmaindarktwo;
			oParam.oRtf->m_oColorTable.AddItem( oNewColor );

		oColorReader.Parse( oParam, m_ooxColorScheme->m_oLt1, oNewColor);
			oNewColor.m_eTheme = RtfColor::cmainlightone;
			oParam.oRtf->m_oColorTable.AddItem( oNewColor );

		oColorReader.Parse( oParam, m_ooxColorScheme->m_oLt2, oNewColor);
			oNewColor.m_eTheme = RtfColor::cmainlighttwo;
			oParam.oRtf->m_oColorTable.AddItem( oNewColor );

		oColorReader.Parse( oParam, m_ooxColorScheme->m_oHlink, oNewColor);
			oNewColor.m_eTheme = RtfColor::chyperlink;
			oParam.oRtf->m_oColorTable.AddItem( oNewColor );

		oColorReader.Parse( oParam, m_ooxColorScheme->m_oFolHlink, oNewColor);
			oNewColor.m_eTheme = RtfColor::cfollowedhyperlink;
			oParam.oRtf->m_oColorTable.AddItem( oNewColor );

		//else if( _T("a:bg1") == sNodeName )
		//	oNewColor.m_eTheme = RtfColor::cbackgroundone;
		//else if( _T("a:bg2") == sNodeName )
		//	oNewColor.m_eTheme = RtfColor::cbackgroundtwo;
		//else if( _T("a:phClr") == sNodeName )
		//	oNewColor.m_eTheme = RtfColor::cmainlighttwo;
		//else if( _T("a:tx1") == sNodeName )
		//	oNewColor.m_eTheme = RtfColor::ctextone;
		//else if( _T("a:tx2") == sNodeName )
		//	oNewColor.m_eTheme = RtfColor::ctexttwo;


		return true;
	}
};