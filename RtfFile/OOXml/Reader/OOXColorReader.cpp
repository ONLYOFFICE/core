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

#include "OOXColorReader.h"
#include "../../../OOXML/PPTXFormat/Logic/Colors/SchemeClr.h"

OOXColorReader::OOXColorReader()
{
	m_ooxColorScheme = NULL;
}
OOXColorReader::OOXColorReader(PPTX::nsTheme::ClrScheme * ooxColorScheme)
{
	m_ooxColorScheme = ooxColorScheme;
}
bool OOXColorReader::Parse( ReaderParameter oParam, const std::wstring &name, RtfColor & oOutputColor )
{
	if (!m_ooxColorScheme) return false;

	std::map<std::wstring, PPTX::Logic::UniColor>::iterator pFind = m_ooxColorScheme->Scheme.find(name);

	if (pFind == m_ooxColorScheme->Scheme.end()) return false;
	PPTX::Logic::UniColor & color = pFind->second;

	switch(color.getType ())
	{
	case OOX::et_a_schemeClr:
	{
		NSCommon::smart_ptr<PPTX::Logic::SchemeClr> schemeColor = color.Color.smart_dynamic_cast<PPTX::Logic::SchemeClr>();
		if (schemeColor.IsInit())
		{
			RtfColor::_ThemeColor oTheme;
			if( true == RtfColor::GetThemeByOOX( (SimpleTypes::EShemeColorVal)schemeColor->val.GetBYTECode(), oTheme ))
				return oParam.oRtf->m_oColorTable.GetColor( oTheme, oOutputColor );
		}
	}break;
	case OOX::et_a_prstClr:
	case OOX::et_a_scrgbClr:
	case OOX::et_a_srgbClr:
	case OOX::et_a_sysClr:
	default:
	{
		DWORD rgba = color.GetRGBA();
		oOutputColor.SetRGB(rgba >> 8);
		return true;
	}break;
	}
	return false;
}
bool OOXColorReader::Parse(ReaderParameter oParam, ComplexTypes::Word::CColor ooxColor, RtfColor & oOutputColor)
{
	bool res = false;
	if (ooxColor.m_oThemeColor.IsInit())
	{
		oParam.oRtf->m_oColorTable.GetColor( ooxColor.m_oThemeColor->GetValue() , oOutputColor );
		if (oOutputColor.m_eTheme != RtfColor::TC_NONE) res = true;
	}
	if (ooxColor.m_oThemeShade.IsInit())
	{
		oOutputColor.SetShade( ooxColor.m_oThemeShade->GetValue() );
		res = true;
	}
	if (ooxColor.m_oThemeTint.IsInit())
	{
		oOutputColor.SetTint( ooxColor.m_oThemeTint->GetValue() );
		res = true;
	}
	if (ooxColor.m_oVal.IsInit())
	{
		if (ooxColor.m_oVal->GetValue() == SimpleTypes::hexcolorRGB)
			oOutputColor.SetRGB(ooxColor.m_oVal->Get_R(), ooxColor.m_oVal->Get_G(), ooxColor.m_oVal->Get_B());
		if (ooxColor.m_oVal->GetValue() == SimpleTypes::hexcolorAuto)
			oOutputColor.m_bAuto = true;

		res = true;
	}
	return res;
}

