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

#include "OOXColorReader.h"

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

