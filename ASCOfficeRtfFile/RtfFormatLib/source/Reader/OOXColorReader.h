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
#include "../RtfProperty.h"
#include "OOXReader.h"
#include "OOXReaderBasic.h"

#include "../../../../Common/DocxFormat/Source/DocxFormat/Drawing/DrawingStyles.h"

class OOXColorReader
{
public: 
	OOXColorReader()
	{
	}
	bool Parse( ReaderParameter oParam, OOX::Drawing::CStyleColor & ooxColor, RtfColor & oOutputColor )
	{	
		if (ooxColor.getType() == OOX::Drawing::colorHsl)
		{
			double dHue, dSat, dLum;
			ooxColor.m_oHslClr.GetHSL(dHue,dSat,dLum);
			oOutputColor.SetHSL( dHue, dSat, dLum );
			return true;
		}
		else if(ooxColor.getType() == OOX::Drawing::colorPrst)
		{
			oOutputColor = RtfColor(ooxColor.m_oPrstClr.m_oVal.Get_R(), ooxColor.m_oPrstClr.m_oVal.Get_G(), ooxColor.m_oPrstClr.m_oVal.Get_B());
			return true;
		}
		else if(ooxColor.getType() == OOX::Drawing::colorSheme)
		{
			RtfColor::ThemeColor oTheme;
			if( true == RtfColor::GetThemeByOOX( ooxColor.m_oShemeClr.m_oVal.GetValue(), oTheme ))
				return oParam.oRtf->m_oColorTable.GetColor( oTheme, oOutputColor );
			return false;
		}
		else if( ooxColor.getType() == OOX::Drawing::colorSRgb)
		{
			oOutputColor = RtfColor(ooxColor.m_oSrgbClr.m_oVal.Get_R(), ooxColor.m_oSrgbClr.m_oVal.Get_G(), ooxColor.m_oSrgbClr.m_oVal.Get_B());
			return true;
		}
		else if( ooxColor.getType() == OOX::Drawing::colorScRgb)
		{
			unsigned char r, g, b, a;
			ooxColor.m_oScrgbClr.GetRGBA( r, g, b, a);
			oOutputColor = RtfColor(r, g, b);
			return true;
		}
		else if( ooxColor.getType() == OOX::Drawing::colorSys)
		{
			//if (ooxColor.m_oSysClr.m_oLastClr)
			//{
			//}
			unsigned char r, g, b, a;
			ooxColor.m_oSysClr.GetRGBA( r, g, b, a);
			oOutputColor = RtfColor(r, g, b);
			return true;
		}
		return false;
	}
	bool Parse (ReaderParameter oParam, ComplexTypes::Word::CColor ooxColor, RtfColor & oOutputColor)
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
		if ((ooxColor.m_oVal.IsInit()) && (ooxColor.m_oVal->GetValue() == SimpleTypes::hexcolorRGB))
		{
			oOutputColor.SetRGB(ooxColor.m_oVal->Get_R(), ooxColor.m_oVal->Get_G(), ooxColor.m_oVal->Get_B());
			res = true;
		}
		return res;

	}
};
