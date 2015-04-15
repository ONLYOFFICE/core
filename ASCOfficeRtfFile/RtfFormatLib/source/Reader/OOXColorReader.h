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
		if (ooxColor.m_oThemeColor.IsInit())
		{
			oParam.oRtf->m_oColorTable.GetColor( ooxColor.m_oThemeColor->GetValue() , oOutputColor );
		}
		if (ooxColor.m_oThemeShade.IsInit())
		{
			oOutputColor.SetShade( ooxColor.m_oThemeShade->GetValue() );
		}
		if (ooxColor.m_oThemeTint.IsInit())
		{
			oOutputColor.SetTint( ooxColor.m_oThemeTint->GetValue() );
		}
		if (ooxColor.m_oVal.IsInit())
		{
			oOutputColor.SetRGB(ooxColor.m_oVal->Get_R(), ooxColor.m_oVal->Get_G(), ooxColor.m_oVal->Get_B());
		}
		return true;

	}
};
