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
#pragma once

#include "RtfProperty.h"
#include "RtfShape.h"
#include <set>

class RtfFontTable : public IDocumentElement, public ItemSingleContainer<RtfFont>
{
public:
	void AddFont( RtfFont piRend);

	bool GetFont( int nId, RtfFont& oFont);
	bool GetFont( std::wstring sName, RtfFont& oFont );

	std::wstring RenderToRtf(RenderParameter oRenderParameter);
	std::wstring RenderToOOX(RenderParameter oRenderParameter);
};

class RtfColorTable : public IDocumentElement, public ItemSingleContainer<RtfColor>
{
public: 
	RtfColorTable();

	void AddColor( RtfColor piRend);
	int AddItem( RtfColor piRend);

	bool GetColor( int nId, RtfColor& oColor);
	bool GetColor( RtfColor::_ThemeColor oTheme, RtfColor& oColor);
	bool GetColor( std::wstring sTheme, RtfColor& oColor);
	bool GetColor( RtfColor oColor , _INT32 & nId);

	std::wstring RenderToRtf(RenderParameter oRenderParameter);
	std::wstring RenderToOOX(RenderParameter oRenderParameter);
};

class RtfStyleTable : public IDocumentElement, public ItemContainer<RtfStylePtr>
{
public: 
	bool GetStyle( int nId, RtfStylePtr& oStyle);
	bool GetStyle( std::wstring sName, RtfStylePtr& oStyle );
	RtfStylePtr GetStyleResulting( RtfStylePtr oInputStyle );
	
	std::wstring RenderToRtf(RenderParameter oRenderParameter);
	std::wstring RenderToOOX(RenderParameter oRenderParameter);

private:
	RtfStylePtr GetStyleResulting( RtfStylePtr oInputStyle, std::set<int>& usedStyles );
};

//class RtfLatentStyleTable : public IDocumentElement, public ItemContainer<RtfStyleException>
//{
//public: int m_nCount;
//		int m_nLocked;
//		int m_nSemiHidden;
//		int m_nUnHiddenWhenUse;
//		int m_nQFormat;
//		int m_nPriority;
//		RtfLatentStyleTable()
//		{
//            m_nCount = PROP_DEF;
//            m_nLocked = PROP_DEF;
//            m_nSemiHidden = PROP_DEF;
//            m_nUnHiddenWhenUse = PROP_DEF;
//            m_nQFormat = PROP_DEF;
//            m_nPriority = PROP_DEF;
//		}
//		bool GetException( std::wstring sName, RtfStyleException& oListProperty )
//		 {
//			 for (size_t i = 0; i < (int)m_aArray.size(); i++ )
//				 if( sName == m_aArray[i].m_sName )
//				 {
//					oListProperty =  m_aArray[i];
//					return true;
//				 }
//			 return false;
//		 }
//		std::wstring RenderToRtf(RenderParameter oRenderParameter)
//		{
//			std::wstring sResult;
//			if( m_aArray.size() > 0 )
//			{
//				sResult += _T("{\\*\\latentstyles"));
//				if( PROP_DEF != m_nCount )
//					sResult.AppendFormat(_T("\\lsdstimax%d"),m_nCount);
//				if( PROP_DEF != m_nLocked )
//					sResult.AppendFormat(_T("\\lsdlockeddef%d"),m_nLocked);
//				if( PROP_DEF != m_nSemiHidden )
//					sResult.AppendFormat(_T("\\lsdsemihiddendef%d"),m_nSemiHidden);
//				if( PROP_DEF != m_nUnHiddenWhenUse )
//					sResult.AppendFormat(_T("\\lsdunhideuseddef%d"),m_nUnHiddenWhenUse);
//				if( PROP_DEF != m_nQFormat )
//					sResult.AppendFormat(_T("\\lsdqformatdef%d"),m_nQFormat);
//				if( PROP_DEF != m_nPriority )
//					sResult.AppendFormat(_T("\\lsdprioritydef%d"),m_nPriority);
//				sResult += _T("{\\lsdlockedexcept "));
//				for (size_t i = 0; i < (int)m_aArray.size(); i++ )
//					sResult += m_aArray[i].RenderToRtf( oRenderParameter ));
//				sResult += _T("}");
//				sResult += _T("}");
//			}
//			return sResult;
//		}
//		std::wstring RenderToOOX(RenderParameter oRenderParameter)
//		{
//			XmlUtils::CXmlWriter oXmlWriter;
//			if( m_aArray.size() > 0 )
//			{
//				oXmlWriter.WriteNodeBegin(_T("w:latentStyles"),1);
//
//				if( PROP_DEF != m_nCount )
//					oXmlWriter.WriteAttribute(_T("w:count"),m_nCount);
//				if( PROP_DEF != m_nLocked )
//					oXmlWriter.WriteAttribute(_T("w:defLockedState"),m_nLocked);
//				if( PROP_DEF != m_nSemiHidden )
//					oXmlWriter.WriteAttribute(_T("w:defSemiHidden"),m_nSemiHidden);
//				if( PROP_DEF != m_nUnHiddenWhenUse )
//					oXmlWriter.WriteAttribute(_T("w:defUnhideWhenUsed"),m_nUnHiddenWhenUse);
//				if( PROP_DEF != m_nQFormat )
//					oXmlWriter.WriteAttribute(_T("w:defQFormat"),m_nQFormat);
//				if( PROP_DEF != m_nPriority )
//					oXmlWriter.WriteAttribute(_T("w:defUIPriority"),m_nPriority);
//				oXmlWriter.WriteNodeEnd(_T("w:latentStyles"),1,0);
//
//				for (size_t i = 0; i < (int)m_aArray.size(); i++ )
//					oXmlWriter.WriteString( m_aArray[i].RenderToOOX(oRenderParameter) );
//
//				oXmlWriter.WriteNodeEnd(_T("w:latentStyles"),0);
//			}
//			return oXmlWriter.GetXmlString();
//		}
//};

class RtfListTable : public IDocumentElement, public ItemContainer<RtfListProperty>
{
public:
	ItemContainer<RtfShapePtr> m_aPictureList;

	bool GetList( std::wstring sName, RtfListProperty& oListProperty );
	bool GetList( int nId, RtfListProperty& oListProperty );

	std::wstring RenderToRtf(RenderParameter oRenderParameter);
	std::wstring RenderToOOX(RenderParameter oRenderParameter);
};

class RtfListOverrideTable : public IDocumentElement, public ItemContainer<RtfListOverrideProperty>
{
public: 
	bool GetList( int nId, RtfListOverrideProperty& oListOverrideProperty );
	std::wstring RenderToRtf(RenderParameter oRenderParameter);
	std::wstring RenderToOOX(RenderParameter oRenderParameter);
};

class RtfRevisionTable : public IDocumentElement, public ItemContainer<std::wstring>
{
public:
	std::wstring RenderToRtf(RenderParameter oRenderParameter);
	std::wstring RenderToOOX(RenderParameter oRenderParameter);

	int AddAuthor(std::wstring author);
	std::wstring GetAuthor(int ind);
};

//class RtfRSIDTable : public IDocumentElement, public ItemContainer<rsidString>
//{
//public:
//
//	std::wstring RenderToRtf(RenderParameter oRenderParameter)
//	{
//		std::wstring sResult;
//		if( m_aArray.size() > 0 )
//		{
//			sResult += _T("{\\*\\rsidtbl ");
//
//			for (size_t i = 0; i < m_aArray.size(); i++)
//            {
//				sResult += _T("{");
//				sResult += m_aArray[i];
// 				sResult += _T("}");
//           }
//			sResult += _T("}");
//		}
//		return sResult;
//	}
//	std::wstring RenderToOOX(RenderParameter oRenderParameter)
//	{
//	}
//};
