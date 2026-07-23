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

#include "RtfTableRow.h"

int RtfTableRow::GetType()
{
	return TYPE_RTF_TABLE_ROW;
}
RtfTableRow::RtfTableRow()
{
}
std::wstring RtfTableRow::RenderToRtf(RenderParameter oRenderParameter)
{
	std::wstring sResult;
	sResult += L"\n";
	if( RENDER_TO_RTF_PARAM_NESTED == oRenderParameter.nType )
	{
		for (size_t i = 0; i < m_aArray.size(); i++ )
		{
			sResult += m_aArray[i]->RenderToRtf( oRenderParameter );
		}
		sResult += L"{\\*\\nesttableprops";
		sResult += m_oProperty.RenderToRtf( oRenderParameter );
		sResult += L"\\nestrow}{\\nonesttables \\par}";
	}
	else
	{
		sResult += m_oProperty.RenderToRtf( oRenderParameter );
		for (size_t i = 0; i < m_aArray.size(); i++ )
		{
			sResult += m_aArray[i]->RenderToRtf( oRenderParameter );
		}
		sResult += L"\\row";
	}
	return sResult;
}
std::wstring RtfTableRow::RenderToOOX(RenderParameter oRenderParameter)
{
	XmlUtils::CXmlWriter oXmlWriter;
	oXmlWriter.WriteNodeBegin(L"w:tr",0);

	RenderParameter oNewParam = oRenderParameter;
	oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;

	std::wstring sRowProp = m_oProperty.RenderToOOX(oNewParam);
	if( false == sRowProp.empty() )
	{
		std::wstring sXml = L"<w:trPr>" + sRowProp + L"</w:trPr>";
		oXmlWriter.WriteString(sXml);
	}
	for (size_t i = 0 ; i < m_aArray.size(); i++)
	{
		oXmlWriter.WriteString( m_aArray[i]->RenderToOOX(oNewParam) );
	}
	oXmlWriter.WriteNodeEnd(L"w:tr");
	return oXmlWriter.GetXmlString();
}
