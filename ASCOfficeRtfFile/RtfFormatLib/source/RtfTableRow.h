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

#include "RtfTableCell.h"

class RtfTableRow : public IDocumentElement, public ItemContainer< RtfTableCellPtr >
{
public: 
	RtfRowProperty m_oProperty;
	int GetType()
	{
		return TYPE_RTF_TABLE_ROW;
	}
	RtfTableRow()
	{
	}
	CString RenderToRtf(RenderParameter oRenderParameter)
	{
		CString sResult;
		sResult.Append(_T("\n"));
		if( RENDER_TO_RTF_PARAM_NESTED == oRenderParameter.nType )
		{

			for( int i = 0; i < (int)m_aArray.size(); i++ )
			{
				sResult.Append( m_aArray[i]->RenderToRtf( oRenderParameter ) );
			}
			sResult.Append(_T("{\\*\\nesttableprops"));
			sResult.Append( m_oProperty.RenderToRtf( oRenderParameter ) );
			sResult.Append(_T("\\nestrow}{\\nonesttables \\par}"));
		}
		else
		{
			sResult.Append( m_oProperty.RenderToRtf( oRenderParameter ) );
			for( int i = 0; i < (int)m_aArray.size(); i++ )
			{
				sResult.Append( m_aArray[i]->RenderToRtf( oRenderParameter ) );
			}
			sResult.Append(_T("\\row"));
		}
		return sResult;
	}
	CString RenderToOOX(RenderParameter oRenderParameter)
	{
		XmlUtils::CXmlWriter oXmlWriter;
		oXmlWriter.WriteNodeBegin(_T("w:tr"),0);
		RenderParameter oNewParam = oRenderParameter;
		oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;

		CString sRowProp = m_oProperty.RenderToOOX(oNewParam);
		if( false == sRowProp.IsEmpty() )
		{
			CString sXml;
			sXml.Format(_T("<w:trPr>%ls</w:trPr>"), sRowProp);
			oXmlWriter.WriteString(sXml);
		}
		for(int i = 0 ; i < (int)m_aArray.size(); i++)
		{
			oXmlWriter.WriteString( m_aArray[i]->RenderToOOX(oNewParam) );
		}
		oXmlWriter.WriteNodeEnd(_T("w:tr"));
		return oXmlWriter.GetXmlString();
	}

};


typedef boost::shared_ptr<RtfTableRow> RtfTableRowPtr;