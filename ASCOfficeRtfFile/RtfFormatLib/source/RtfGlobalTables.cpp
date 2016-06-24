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
#include "RtfGlobalTables.h"
#include "RtfDocument.h"

CString RtfFontTable::RenderToRtf(RenderParameter oRenderParameter)
{
	////записывает default charset и codepage
	//RtfDocument* poDocument = static_cast<RtfDocument*>( oRenderParameter.poDocument );
	//if( PROP_DEF != poDocument->m_oProperty.m_nAnsiCodePage )
	//{
	//	int nCodepage = poDocument->m_oProperty.m_nAnsiCodePage;
	//	int nCharset = RtfUtility::CodepageToCharset( nCodepage );
	//	for( int i = 0; i < (int)m_aArray.size(); i++ )
	//		{
	//			m_aArray[i].m_nCharset = nCharset;
	//			m_aArray[i].m_nCodePage = nCodepage;
	//		}
	//}
	
	CString sResult;
	if( m_aArray.size() > 0 )
	{
		sResult.Append(_T("{\\fonttbl"));
		RenderParameter oNewParameter = oRenderParameter;
		oNewParameter.nType = RENDER_TO_RTF_PARAM_FONT_TBL;
		for( int i = 0; i < (int)m_aArray.size(); i++ )
		{
            sResult += m_aArray[i].RenderToRtf( oNewParameter );
		}
		sResult += _T("}");
	}
	return sResult;
}
CString RtfListTable::RenderToOOX(RenderParameter oRenderParameter)
{
	CString sResult;
	if( m_aArray.size() > 0 )
	{
		RenderParameter oNewParam = oRenderParameter;
		oNewParam.nType = RENDER_TO_OOX_PARAM_SHAPE_WSHAPE;
		for( int i = 0; i < (int)m_aPictureList.GetCount(); i++ )
		{
			sResult.AppendFormat(_T("<w:numPicBullet w:numPicBulletId=\"%d\">"), i );
			sResult.Append( m_aPictureList[i]->RenderToOOX(oNewParam) );
			sResult.Append(_T("</w:numPicBullet>") );
		}
		for( int i = 0; i < (int)m_aArray.size(); i++)
			sResult.Append( m_aArray[i].RenderToOOX(oRenderParameter) );
	}
	else
	{
		RenderParameter oNewParam = oRenderParameter;
		oNewParam.nType = RENDER_TO_OOX_PARAM_OLDLIST_ABS;
		RtfDocument* poDocument = static_cast<RtfDocument*>( oRenderParameter.poDocument );
		for( int i = 0; i < (int)poDocument->m_aOldLists.size(); i++ )
			sResult.Append( poDocument->m_aOldLists[i]->RenderToOOX( oNewParam ) );
	}
	return sResult;
}
CString RtfListOverrideTable::RenderToOOX(RenderParameter oRenderParameter)
{
	CString sResult;
	if( m_aArray.size() > 0 )
	{
		RenderParameter oNewParam = oRenderParameter;
		oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;
		for( int i = 0; i < (int)m_aArray.size(); i++)
			sResult.Append( m_aArray[i].RenderToOOX(oNewParam) );
	}
	else
	{
		RenderParameter oNewParam = oRenderParameter;
		oNewParam.nType = RENDER_TO_OOX_PARAM_OLDLIST_OVR;
		RtfDocument* poDocument = static_cast<RtfDocument*>( oRenderParameter.poDocument );
		for( int i = 0; i < (int)poDocument->m_aOldLists.size(); i++ )
			sResult.Append( poDocument->m_aOldLists[i]->RenderToOOX( oNewParam ) );
	}
	return sResult;
}
