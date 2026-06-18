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
#include "RtfGlobalTables.h"
#include "RtfDocument.h"

void RtfFontTable::AddFont( RtfFont piRend)
{
	m_aArray.push_back(piRend);
}
bool RtfFontTable::GetFont( int nId, RtfFont& oFont)
{
	for (size_t i = 0; i < m_aArray.size(); i++ )
	{
		if( nId == m_aArray[i].m_nID )
		{
			m_aArray[i].m_bUsed = true;
			oFont =  m_aArray[i];
			return true;
		}
	}
	return false;
}
bool RtfFontTable::GetFont( std::wstring sName, RtfFont& oFont )
{
	for (size_t i = 0; i < m_aArray.size(); i++ )
	{
		if( sName == m_aArray[i].m_sName )
		{
			m_aArray[i].m_bUsed = true;
			oFont =  m_aArray[i];
			return true;
		}
	}
	return false;
}
std::wstring RtfFontTable::RenderToOOX(RenderParameter oRenderParameter)
{
	if (m_aArray.empty()) return L"";

	std::wstring sResult;
	std::map<std::wstring, bool> mapFontsDouble;	
	
	for (size_t i = 0; i < m_aArray.size(); i++)
	{
		std::map<std::wstring, bool>::iterator pFind = mapFontsDouble.find(m_aArray[i].m_sName);
		if (pFind == mapFontsDouble.end())
		{
			//if (m_aArray[i].m_bUsed)
			sResult += m_aArray[i].RenderToOOX(oRenderParameter);
			mapFontsDouble.insert(std::make_pair(m_aArray[i].m_sName, m_aArray[i].m_bUsed));
		}
	}
	return sResult;
}
std::wstring RtfFontTable::RenderToRtf(RenderParameter oRenderParameter)
{
	////writes default charset and codepage
	//RtfDocument* poDocument = static_cast<RtfDocument*>( oRenderParameter.poDocument );
	//if( PROP_DEF != poDocument->m_oProperty.m_nAnsiCodePage )
	//{
	//	int nCodepage = poDocument->m_oProperty.m_nAnsiCodePage;
	//	int nCharset = RtfUtility::CodepageToCharset( nCodepage );
	//	for (size_t i = 0; i < (int)m_aArray.size(); i++ )
	//		{
	//			m_aArray[i].m_nCharset = nCharset;
	//			m_aArray[i].m_nCodePage = nCodepage;
	//		}
	//}
	
	std::wstring sResult;
	if( m_aArray.size() > 0 )
	{
		sResult += _T("{\\fonttbl");
		RenderParameter oNewParameter = oRenderParameter;
		oNewParameter.nType = RENDER_TO_RTF_PARAM_FONT_TBL;
		
		for (size_t i = 0; i < m_aArray.size(); i++ )
		{
			sResult += m_aArray[i].RenderToRtf( oNewParameter );
		}
		sResult += _T("}");
	}
	return sResult;
}

RtfColorTable::RtfColorTable()
{
}
void RtfColorTable::AddColor( RtfColor piRend)
{
	m_aArray.push_back(piRend);
}
int RtfColorTable::AddItem( RtfColor piRend)
{
	int nIndex = ItemSingleContainer<RtfColor>::AddItem( piRend );
	return nIndex + 1;
}
bool RtfColorTable::GetColor( int nId, RtfColor& oColor)
{
	if( nId >= 0 && nId < (int)m_aArray.size() )
	{
		oColor = m_aArray[nId];
		return true;
	}
	return false;
}
bool RtfColorTable::GetColor( RtfColor::_ThemeColor oTheme, RtfColor& oColor)
{
	for (size_t i = 0; i < m_aArray.size(); i++ )
		if( oTheme == m_aArray[i].m_eTheme )
		{
			oColor = m_aArray[i];
			return true;
		}
	return false;
}
bool RtfColorTable::GetColor( std::wstring sTheme, RtfColor& oColor)
{
	RtfColor::_ThemeColor oTheme = RtfColor::TC_NONE;

	if( true == RtfColor::GetThemeByString(sTheme,oTheme  ) )
	{
		for (size_t i = 0; i < m_aArray.size(); i++ )
		{
			if( oTheme == m_aArray[i].m_eTheme )
			{
				oColor = m_aArray[i];
				return true;
			}
		}
	}
	return false;
}
bool RtfColorTable::GetColor( RtfColor oColor , _INT32 & nId)
{
	for (size_t i = 0; i < m_aArray.size(); i++ )
	{
		if( m_aArray[i] == oColor )
		{
			nId = (int)i + 1;
			return true;
		}
	}
	return false;
}
std::wstring RtfColorTable::RenderToRtf(RenderParameter oRenderParameter)
{
	std::wstring sResult;
	if( m_aArray.size() > 0 )
	{
		sResult += _T("{\\colortbl;");
		RenderParameter oNewParameter = oRenderParameter;
		oNewParameter.nType = RENDER_TO_RTF_PARAM_COLOR_TBL;

		for (size_t i = 0; i < m_aArray.size(); i++ )
		{
			sResult += m_aArray[i].RenderToRtf( oNewParameter );
		}

		sResult += _T("}");
	}
	return sResult;
}
std::wstring RtfColorTable::RenderToOOX(RenderParameter oRenderParameter)
{
	return _T("");
}

bool RtfStyleTable::GetStyle( int nId, RtfStylePtr& oStyle)
{
	for (size_t i = 0; i < m_aArray.size(); i++ )
	{
		if( nId == m_aArray[i]->m_nID )
		{
			oStyle =  m_aArray[i];
			return true;
		}
	}
	return false;
}
bool RtfStyleTable::GetStyle( std::wstring sName, RtfStylePtr& oStyle )
{
	for (size_t i = 0; i < m_aArray.size(); i++ )
	{
		if( sName == m_aArray[i]->m_sID )
		{
			oStyle =  m_aArray[i];
			return true;
		}
	}
	return false;
}
RtfStylePtr RtfStyleTable::GetStyleResulting( RtfStylePtr oInputStyle )
{
	std::set<int> usedStyles;
	return GetStyleResulting( oInputStyle, usedStyles );
}
RtfStylePtr RtfStyleTable::GetStyleResulting( RtfStylePtr oInputStyle, std::set<int>& usedStyles )
{
	RtfStylePtr				oResultStyle;
	RtfStyle::_StyleType	eStyleType	= RtfStyle::st_none;

	int nStyleId = oInputStyle->m_nID;
	int nLinked = PROP_DEF;
	int nBaseOn = oInputStyle->m_nBasedOn;

	usedStyles.insert( nStyleId );

	if( RtfStyle::stCharacter == oInputStyle->m_eType )
	{
		eStyleType		= RtfStyle::stCharacter;
		oResultStyle	= RtfCharStylePtr( new RtfCharStyle() );
	}
	else if( RtfStyle::stParagraph == oInputStyle->m_eType )
	{
		eStyleType		= RtfStyle::stParagraph;
		oResultStyle	= RtfParagraphStylePtr( new RtfParagraphStyle() );
		nLinked			= oInputStyle->m_nLink;//linked will only be checked for paragraph styles to avoid recursion
	}
	else if( RtfStyle::stTable == oInputStyle->m_eType )
	{
		eStyleType		= RtfStyle::stTable;
		oResultStyle	= RtfTableStylePtr( new RtfTableStyle() );
	}
	else
		return oInputStyle;	//DANGEROUS.. can be overwritten by other later todooo

	RtfStylePtr oLinkedStyle;
	//if( PROP_DEF != nLinked && nStyleId != nLinked)
	//{
	// RtfStylePtr oTemStyle;
	// if( true == GetStyle( nLinked, oTemStyle) )
	//	oLinkedStyle = GetStyleResulting( oTemStyle );
	//}
	RtfStylePtr oBaseStyle;
	if( PROP_DEF != nBaseOn && nStyleId != nBaseOn && usedStyles.find( nBaseOn ) == usedStyles.end())
	{
		RtfStylePtr oTemStyle;
		if( true == GetStyle( nBaseOn, oTemStyle) )
			oBaseStyle = GetStyleResulting( oTemStyle, usedStyles );
	}

	//Experimentally established - Base is older than Link
	if( NULL != oLinkedStyle )
	{
		oResultStyle->Merge( oLinkedStyle );
	}
	if( NULL != oBaseStyle )
	{
		oResultStyle->Merge( oBaseStyle );
	}
	oResultStyle->Merge( oInputStyle );

	return oResultStyle;
}
std::wstring RtfStyleTable::RenderToRtf(RenderParameter oRenderParameter)
{
	std::wstring sResult;
	if( m_aArray.size() > 0 )
	{
		sResult += _T("{\\stylesheet");

		for (size_t i = 0; i < m_aArray.size(); i++ )
		{
			std::wstring str = m_aArray[i]->RenderToRtf( oRenderParameter );
			sResult += str + _T("\n\n");
		}

		sResult += _T("}");
	}
	return sResult;
}
std::wstring RtfStyleTable::RenderToOOX(RenderParameter oRenderParameter)
{
	std::wstring sResult;
	for (size_t i = 0; i < m_aArray.size(); i++ )
	{
		sResult += m_aArray[i]->RenderToOOX(oRenderParameter);
	}
	return sResult;
}

bool RtfListTable::GetList( std::wstring sName, RtfListProperty& oListProperty )
{
	for (size_t i = 0; i < m_aArray.size(); i++ )
	{
		if( sName == m_aArray[i].m_sName )
		{
			oListProperty =  m_aArray[i];
			return true;
		}
	}
	return false;
}
bool RtfListTable::GetList( int nId, RtfListProperty& oListProperty )
{
	for (size_t i = 0; i < m_aArray.size(); i++ )
	{
		if( nId == m_aArray[i].m_nID )
		{
			oListProperty =  m_aArray[i];
			return true;
		}
	}
	return false;
}
std::wstring RtfListTable::RenderToRtf(RenderParameter oRenderParameter)
{
	std::wstring sResult;
	if( m_aArray.size() > 0 )
	{
		sResult += _T("{\\*\\listtable ");
		if( m_aPictureList.GetCount() > 0 )
		{
			sResult += _T("{\\*\\listpicture");
			for (int i = 0; i < m_aPictureList.GetCount(); i++ )
			{
				sResult +=  m_aPictureList[i]->RenderToRtf( oRenderParameter );
			}
			sResult += _T("}");
		}
		for (size_t i = 0; i < m_aArray.size(); i++)
		{
			sResult += _T("{");
			sResult += m_aArray[i].RenderToRtf( oRenderParameter );
			sResult += _T("}");
		}
		sResult += _T("}");
	}
	return sResult;
}
std::wstring RtfListTable::RenderToOOX(RenderParameter oRenderParameter)
{
	std::wstring sResult;
	if( m_aArray.size() > 0 )
	{
		RenderParameter oNewParam = oRenderParameter;
		oNewParam.nType = RENDER_TO_OOX_PARAM_PIC_BULLET;
		for (int i = 0; i < m_aPictureList.GetCount(); i++ )
		{
			sResult += L"<w:numPicBullet w:numPicBulletId=\"" + std::to_wstring(i) + L"\">";
			sResult += m_aPictureList[i]->RenderToOOX(oNewParam);
			sResult += L"</w:numPicBullet>";
		}
		for (size_t i = 0; i < m_aArray.size(); i++)
			sResult += m_aArray[i].RenderToOOX(oRenderParameter);
	}
	else
	{
		RenderParameter oNewParam = oRenderParameter;
		oNewParam.nType = RENDER_TO_OOX_PARAM_OLDLIST_ABS;
		RtfDocument* poDocument = static_cast<RtfDocument*>( oRenderParameter.poDocument );
		
		for (size_t i = 0; i < poDocument->m_aOldLists.size(); i++ )
		{
			sResult += poDocument->m_aOldLists[i]->RenderToOOX( oNewParam );
		}
	}
	return sResult;
}

bool RtfListOverrideTable::GetList( int nId, RtfListOverrideProperty& oListOverrideProperty )
{
	for (size_t i = 0; i < m_aArray.size(); i++ )
	{
		if( nId == m_aArray[i].m_nIndex )
		{
			oListOverrideProperty =  m_aArray[i];
			return true;
		}
	}
	return false;
}
std::wstring RtfListOverrideTable::RenderToRtf(RenderParameter oRenderParameter)
{
	std::wstring sResult;
	if( m_aArray.size() > 0 )
	{
		sResult += _T("{\\*\\listoverridetable");
		for (size_t i = 0; i < m_aArray.size(); i++)
		{
			sResult += _T("{");
			sResult += m_aArray[i].RenderToRtf( oRenderParameter );
			sResult += _T("}");
		}
		sResult += _T("}");
	}
	return sResult;
}
std::wstring RtfListOverrideTable::RenderToOOX(RenderParameter oRenderParameter)
{
	std::wstring sResult;
	if( !m_aArray.empty())
	{
		RenderParameter oNewParam = oRenderParameter;
		oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;
		
		for (size_t i = 0; i < m_aArray.size(); i++)
			sResult += m_aArray[i].RenderToOOX(oNewParam);
	}
	else
	{
		RenderParameter oNewParam = oRenderParameter;
		oNewParam.nType = RENDER_TO_OOX_PARAM_OLDLIST_OVR;
		RtfDocument* poDocument = static_cast<RtfDocument*>( oRenderParameter.poDocument );
		
		for (size_t i = 0; i < poDocument->m_aOldLists.size(); i++ )
			sResult += poDocument->m_aOldLists[i]->RenderToOOX( oNewParam );
	}
	return sResult;
}

std::wstring RtfRevisionTable::RenderToRtf(RenderParameter oRenderParameter)
{
	if (m_aArray.empty()) return L"";

	std::wstring sResult;

	sResult += _T("{\\*\\revtbl ");

	sResult += L"{Unknown;}";
	
	RenderParameter oNewParam = oRenderParameter;
	oNewParam.nType = RENDER_TO_RTF_PARAM_CHAR;

	for (size_t i = 0; i < m_aArray.size(); i++)
	{
		sResult += _T("{");
		RtfCharPtr pNewChar(new RtfChar());

		pNewChar->setText(m_aArray[i]);
		sResult += pNewChar->RenderToRtf(oNewParam);

		//sResult += m_aArray[i]

		sResult += L";}";
	}
	sResult += _T("}");

	return sResult;
}
std::wstring RtfRevisionTable::RenderToOOX(RenderParameter oRenderParameter)
{
	return L"";
}
int RtfRevisionTable::AddAuthor(std::wstring author)
{
	int i = Find(author);
	if (i < 0)
		i = AddItem(author);
	return i;
}
std::wstring RtfRevisionTable::GetAuthor(int ind)
{
	if (ind == PROP_DEF || ind > (int)m_aArray.size())
		return L"";

	return XmlUtils::EncodeXmlStringExtend(m_aArray[ind]);
}
