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

#include "Basic.h"
#include "RtfProperty.h"

class RtfTableCell :  public ITextItemContainer
{
public: 
	RtfCellProperty m_oProperty;
	
	int GetType( )
	{
		return TYPE_RTF_TABLE_CELL;
	}

	RtfTableCell()
	{
	}
	CString RenderToRtf(RenderParameter oRenderParameter)
	{
		CString result;

		for(int i =0 ; i < (int)m_aArray.size();i++)
		{
			if( m_aArray[i]->GetType() == TYPE_RTF_PARAGRAPH )
			{
				result.Append( m_aArray[i]->RenderToRtf( oRenderParameter ) );
				if( i != m_aArray.size() - 1 )
					result.Append( _T("\\par") );
			}
			else
			{
				RenderParameter oNewParameter = oRenderParameter;
				oNewParameter.nType = RENDER_TO_RTF_PARAM_NESTED;
				result.Append( m_aArray[i]->RenderToRtf( oNewParameter ) );
			}


		}
		if( RENDER_TO_RTF_PARAM_NESTED != oRenderParameter.nType )
			result.Append( _T("\\cell"));
		else
			result.Append( _T("\\nestcell{\\nonesttables  }")); //todo как бы вернуть
		return result;
	}
	CString RenderToOOX(RenderParameter oRenderParameter)
	{
		CString sResult;
		sResult.Append(_T("<w:tc>"));

		CString sProp = m_oProperty.RenderToOOX( oRenderParameter );
		if( _T("") != sProp )
		{
			sResult.Append(_T("<w:tcPr>"));
			sResult.Append(sProp);
			sResult.Append(_T("</w:tcPr>"));
		}

		for( int i = 0; i < (int)m_aArray.size(); i++ )
		{
			sResult.Append( m_aArray[i]->RenderToOOX( oRenderParameter) );
		}
		sResult.Append(_T("</w:tc>"));
		return sResult;
	}
};

typedef boost::shared_ptr<RtfTableCell> RtfTableCellPtr;