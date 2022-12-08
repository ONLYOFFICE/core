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

#include "RtfTableCell.h"
	
RtfTableCell::RtfTableCell()
{
}
int RtfTableCell::GetType( )
{
	return TYPE_RTF_TABLE_CELL;
}
std::wstring RtfTableCell::RenderToRtf(RenderParameter oRenderParameter)
{
	std::wstring result;

	for (size_t i =0 ; i < m_aArray.size(); i++)
	{
		if( m_aArray[i]->GetType() == TYPE_RTF_PARAGRAPH )
		{
			result += m_aArray[i]->RenderToRtf( oRenderParameter );

			if( i != m_aArray.size() - 1 )
				result += L"\\par";
		}
		else
		{
			RenderParameter oNewParameter = oRenderParameter;
			oNewParameter.nType = RENDER_TO_RTF_PARAM_NESTED;

			result += m_aArray[i]->RenderToRtf( oNewParameter );
		}


	}
	if( RENDER_TO_RTF_PARAM_NESTED != oRenderParameter.nType )
		result += L"\\cell";
	else
		result += L"\\nestcell{\\nonesttables  }"; //todo как бы вернуть
	return result;
}
std::wstring RtfTableCell::RenderToOOX(RenderParameter oRenderParameter)
{
	std::wstring sResult = L"<w:tc>";

	std::wstring sProp = m_oProperty.RenderToOOX( oRenderParameter );
	if( !sProp.empty() )
	{
		sResult += L"<w:tcPr>";
			sResult += sProp;
		sResult += L"</w:tcPr>";
	}

	for (size_t i = 0; i < m_aArray.size(); i++ )
	{
		sResult += m_aArray[i]->RenderToOOX( oRenderParameter);
	}
	sResult += L"</w:tc>";
	return sResult;
}
