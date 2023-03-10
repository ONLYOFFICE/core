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
	std::wstring sResult;
	if( !m_aArray.empty())
	{
		for (size_t i = 0; i < m_aArray.size(); i++)
		{
			if (m_aArray[i].m_bUsed)
				sResult += m_aArray[i].RenderToOOX(oRenderParameter);
		}

	}
	return sResult;
}
std::wstring RtfFontTable::RenderToRtf(RenderParameter oRenderParameter)
{
	////записывает default charset и codepage
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
	RtfStylePtr				oResultStyle;
	RtfStyle::_StyleType	eStyleType	= RtfStyle::st_none;

	int nStyleId = oInputStyle->m_nID;
	int nLinked = PROP_DEF;
	int nBaseOn = oInputStyle->m_nBasedOn;

	if( RtfStyle::stCharacter == oInputStyle->m_eType )
	{
		eStyleType		= RtfStyle::stCharacter;
		oResultStyle	= RtfCharStylePtr( new RtfCharStyle() );
	}
	else if( RtfStyle::stParagraph == oInputStyle->m_eType )
	{
		eStyleType		= RtfStyle::stParagraph;
		oResultStyle	= RtfParagraphStylePtr( new RtfParagraphStyle() );
		nLinked			= oInputStyle->m_nLink;//linked будем смотреть только у стилей параграфа, чтобы избежать рекурсии
	}
	else if( RtfStyle::stTable == oInputStyle->m_eType )
	{
		eStyleType		= RtfStyle::stTable;
		oResultStyle	= RtfTableStylePtr( new RtfTableStyle() );
	}
	else
		return oInputStyle;	//ОПАСНО .. потом может другим затереться todooo

	RtfStylePtr oLinkedStyle;
	//if( PROP_DEF != nLinked && nStyleId != nLinked)
	//{
	// RtfStylePtr oTemStyle;
	// if( true == GetStyle( nLinked, oTemStyle) )
	//	oLinkedStyle = GetStyleResulting( oTemStyle );
	//}
	RtfStylePtr oBaseStyle;
	if( PROP_DEF != nBaseOn && nStyleId != nBaseOn)
	{
		RtfStylePtr oTemStyle;
		if( true == GetStyle( nBaseOn, oTemStyle) )
			oBaseStyle = GetStyleResulting( oTemStyle );
	}

	//Опытным путем установлено - Base старше Link
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
		oNewParam.nType = RENDER_TO_OOX_PARAM_SHAPE_WSHAPE;
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
