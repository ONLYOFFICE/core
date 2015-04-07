#ifndef _WMF_COLOR_H
#define _WMF_COLOR_H

#include "WmfTypes.h"
#include <stdio.h>

//---------------------------------------------------------------------------------------------------
// Именнованые цвета
//---------------------------------------------------------------------------------------------------

const TWmfRGB c_oColorAliceblue             = { 240, 248, 255 };
const TWmfRGB c_oColorAntiquewhite          = { 250, 235, 215 };
const TWmfRGB c_oColorAqua                  = {   0, 255, 255 };
const TWmfRGB c_oColorAquamarine            = { 127, 255, 212 };
const TWmfRGB c_oColorAzure                 = { 240, 255, 255 };
const TWmfRGB c_oColorBeige                 = { 245, 245, 220 };
const TWmfRGB c_oColorBisque                = { 255, 228, 196 };
const TWmfRGB c_oColorBlack                 = {   0,   0,   0 };
const TWmfRGB c_oColorBlanchedalmond        = { 255, 235, 205 };
const TWmfRGB c_oColorBlue                  = {   0,   0, 255 };
const TWmfRGB c_oColorBlueviolet            = { 138,  43, 226 };
const TWmfRGB c_oColorBrown                 = { 165,  42,  42 };
const TWmfRGB c_oColorBurlywood             = { 222, 184, 135 };
const TWmfRGB c_oColorCadetblue             = {  95, 158, 160 };
const TWmfRGB c_oColorChartreuse            = { 127, 255,   0 };
const TWmfRGB c_oColorChocolate             = { 210, 105,  30 };
const TWmfRGB c_oColorCoral                 = { 255, 127,  80 };
const TWmfRGB c_oColorCornflowerblue        = { 100, 149, 237 };
const TWmfRGB c_oColorCornsilk              = { 255, 248, 220 };
const TWmfRGB c_oColorCrimson               = { 220,  20,  60 };
const TWmfRGB c_oColorCyan                  = {   0, 255, 255 };
const TWmfRGB c_oColorDarkblue              = {   0,   0, 139 };
const TWmfRGB c_oColorDarkcyan              = {   0, 139, 139 };
const TWmfRGB c_oColorDarkgoldenrod         = { 184, 134,  11 };
const TWmfRGB c_oColorDarkgray              = { 169, 169, 169 };
const TWmfRGB c_oColorDarkgreen             = {   0, 100,   0 };
const TWmfRGB c_oColorDarkgrey              = { 169, 169, 169 };
const TWmfRGB c_oColorDarkkhaki             = { 189, 183, 107 };
const TWmfRGB c_oColorDarkmagenta           = { 139,   0, 139 };
const TWmfRGB c_oColorDarkolivegreen        = {  85, 107,  47 };
const TWmfRGB c_oColorDarkorange            = { 255, 140,   0 };
const TWmfRGB c_oColorDarkorchid            = { 153,  50, 204 };
const TWmfRGB c_oColorDarkred               = { 139,   0,   0 };
const TWmfRGB c_oColorDarksalmon            = { 233, 150, 122 };
const TWmfRGB c_oColorDarkseagreen          = { 143, 188, 143 };
const TWmfRGB c_oColorDarkslateblue         = {  72,  61, 139 };
const TWmfRGB c_oColorDarkslategray         = {  47,  79,  79 };
const TWmfRGB c_oColorDarkslategrey         = {  47,  79,  79 };
const TWmfRGB c_oColorDarkturquoise         = {   0, 206, 209 };
const TWmfRGB c_oColorDarkviolet            = { 148,   0, 211 };
const TWmfRGB c_oColorDeeppink              = { 255,  20, 147 };
const TWmfRGB c_oColorDeepskyblue           = {   0, 191, 255 };
const TWmfRGB c_oColorDimgray               = { 105, 105, 105 };
const TWmfRGB c_oColorDimgrey               = { 105, 105, 105 };
const TWmfRGB c_oColorDodgerblue            = {  30, 144, 255 };
const TWmfRGB c_oColorFirebrick             = { 178,  34,  34 };
const TWmfRGB c_oColorFloralwhite           = { 255, 250, 240 };
const TWmfRGB c_oColorForestgreen           = {  34, 139,  34 };
const TWmfRGB c_oColorFuchsia               = { 255,   0, 255 };
const TWmfRGB c_oColorGainsboro             = { 220, 220, 220 };
const TWmfRGB c_oColorGhostwhite            = { 248, 248, 255 };
const TWmfRGB c_oColorGold                  = { 255, 215,   0 };
const TWmfRGB c_oColorGoldenrod             = { 218, 165,  32 };
const TWmfRGB c_oColorGray                  = { 128, 128, 128 };
const TWmfRGB c_oColorGrey                  = { 128, 128, 128 };
const TWmfRGB c_oColorGreen                 = {   0, 128,   0 };
const TWmfRGB c_oColorGreenyellow           = { 173, 255,  47 };
const TWmfRGB c_oColorHoneydew              = { 240, 255, 240 };
const TWmfRGB c_oColorHotpink               = { 255, 105, 180 };
const TWmfRGB c_oColorIndianred             = { 205,  92,  92 };
const TWmfRGB c_oColorIndigo                = {  75,   0, 130 };
const TWmfRGB c_oColorIvory                 = { 255, 255, 240 };
const TWmfRGB c_oColorKhaki                 = { 240, 230, 140 };
const TWmfRGB c_oColorLavender              = { 230, 230, 250 };
const TWmfRGB c_oColorLavenderblush         = { 255, 240, 245 };
const TWmfRGB c_oColorLawngreen             = { 124, 252,   0 };
const TWmfRGB c_oColorLemonchiffon          = { 255, 250, 205 };
const TWmfRGB c_oColorLightblue             = { 173, 216, 230 };
const TWmfRGB c_oColorLightcoral            = { 240, 128, 128 };
const TWmfRGB c_oColorLightcyan             = { 224, 255, 255 };
const TWmfRGB c_oColorLightgoldenrodyellow  = { 250, 250, 210 };
const TWmfRGB c_oColorLightgray             = { 211, 211, 211 };
const TWmfRGB c_oColorLightgreen            = { 144, 238, 144 };
const TWmfRGB c_oColorLightgrey             = { 211, 211, 211 };
const TWmfRGB c_oColorLightpink             = { 255, 182, 193 };
const TWmfRGB c_oColorLightsalmon           = { 255, 160, 122 };
const TWmfRGB c_oColorLightseagreen         = {  32, 178, 170 };
const TWmfRGB c_oColorLightskyblue          = { 135, 206, 250 };
const TWmfRGB c_oColorLightslategray        = { 119, 136, 153 };
const TWmfRGB c_oColorLightslategrey        = { 119, 136, 153 };
const TWmfRGB c_oColorLightsteelblue        = { 176, 196, 222 };
const TWmfRGB c_oColorLightyellow           = { 255, 255, 224 };
const TWmfRGB c_oColorLime                  = {   0, 255,   0 };
const TWmfRGB c_oColorLimegreen             = {  50, 205,  50 };
const TWmfRGB c_oColorLinen                 = { 250, 240, 230 };
const TWmfRGB c_oColorMagenta               = { 255,   0, 255 };
const TWmfRGB c_oColorMaroon                = { 128,   0,   0 };
const TWmfRGB c_oColorMediumaquamarine      = { 102, 205, 170 };
const TWmfRGB c_oColorMediumblue            = {   0,   0, 205 };
const TWmfRGB c_oColorMediumorchid          = { 186,  85, 211 };
const TWmfRGB c_oColorMediumpurple          = { 147, 112, 219 };
const TWmfRGB c_oColorMediumseagreen        = {  60, 179, 113 };
const TWmfRGB c_oColorMediumslateblue       = { 123, 104, 238 };
const TWmfRGB c_oColorMediumspringgreen     = {   0, 250, 154 };
const TWmfRGB c_oColorMediumturquoise       = {  72, 209, 204 };
const TWmfRGB c_oColorMediumvioletred       = { 199,  21, 133 };
const TWmfRGB c_oColorMidnightblue          = {  25,  25, 112 };
const TWmfRGB c_oColorMintcream             = { 245, 255, 250 };
const TWmfRGB c_oColorMistyrose             = { 255, 228, 225 };
const TWmfRGB c_oColorMoccasin              = { 255, 228, 181 };
const TWmfRGB c_oColorNavajowhite           = { 255, 222, 173 };
const TWmfRGB c_oColorNavy                  = {   0,   0, 128 };
const TWmfRGB c_oColorOldlace               = { 253, 245, 230 };
const TWmfRGB c_oColorOlive                 = { 128, 128,   0 };
const TWmfRGB c_oColorOlivedrab             = { 107, 142,  35 };
const TWmfRGB c_oColorOrange                = { 255, 165,   0 };
const TWmfRGB c_oColorOrangered             = { 255,  69,   0 };
const TWmfRGB c_oColorOrchid                = { 218, 112, 214 };
const TWmfRGB c_oColorPalegoldenrod         = { 238, 232, 170 };
const TWmfRGB c_oColorPalegreen             = { 152, 251, 152 };
const TWmfRGB c_oColorPaleturquoise         = { 175, 238, 238 };
const TWmfRGB c_oColorPalevioletred         = { 219, 112, 147 };
const TWmfRGB c_oColorPapayawhip            = { 255, 239, 213 };
const TWmfRGB c_oColorPeachpuff             = { 255, 218, 185 };
const TWmfRGB c_oColorPeru                  = { 205, 133,  63 };
const TWmfRGB c_oColorPink                  = { 255, 192, 203 };
const TWmfRGB c_oColorPlum                  = { 221, 160, 221 };
const TWmfRGB c_oColorPowderblue            = { 176, 224, 230 };
const TWmfRGB c_oColorPurple                = { 128,   0, 128 };
const TWmfRGB c_oColorRed                   = { 255,   0,   0 };
const TWmfRGB c_oColorRosybrown             = { 188, 143, 143 };
const TWmfRGB c_oColorRoyalblue             = {  65, 105, 225 };
const TWmfRGB c_oColorSaddlebrown           = { 139,  69,  19 };
const TWmfRGB c_oColorSalmon                = { 250, 128, 114 };
const TWmfRGB c_oColorSandybrown            = { 244, 164,  96 };
const TWmfRGB c_oColorSeagreen              = {  46, 139,  87 };
const TWmfRGB c_oColorSeashell              = { 255, 245, 238 };
const TWmfRGB c_oColorSienna                = { 160,  82,  45 };
const TWmfRGB c_oColorSilver                = { 192, 192, 192 };
const TWmfRGB c_oColorSkyblue               = { 135, 206, 235 };
const TWmfRGB c_oColorSlateblue             = { 106,  90, 205 };
const TWmfRGB c_oColorSlategray             = { 112, 128, 144 };
const TWmfRGB c_oColorSlategrey             = { 112, 128, 144 };
const TWmfRGB c_oColorSnow                  = { 255, 250, 250 };
const TWmfRGB c_oColorSpringgreen           = {   0, 255, 127 };
const TWmfRGB c_oColorSteelblue             = {  70, 130, 180 };
const TWmfRGB c_oColorTan                   = { 210, 180, 140 };
const TWmfRGB c_oColorTeal                  = {   0, 128, 128 };
const TWmfRGB c_oColorThistle               = { 216, 191, 216 };
const TWmfRGB c_oColorTomato                = { 255,  99,  71 };
const TWmfRGB c_oColorTurquoise             = {  64, 224, 208 };
const TWmfRGB c_oColorViolet                = { 238, 130, 238 };
const TWmfRGB c_oColorWheat                 = { 245, 222, 179 };
const TWmfRGB c_oColorWhite                 = { 255, 255, 255 };
const TWmfRGB c_oColorWhitesmoke            = { 245, 245, 245 };
const TWmfRGB c_oColorYellow                = { 255, 255,   0 };
const TWmfRGB c_oColorYellowgreen           = { 154, 205,  50 };

//---------------------------------------------------------------------------------------------------
// CWmfColor
//---------------------------------------------------------------------------------------------------

class CWmfColor
{
public:

	CWmfColor()
	{
		m_pError = NULL;

        m_ulmax   = 32;
		m_ulCount = 0;

        m_pRGB = (TWmfRGB *)malloc(m_ulmax * sizeof(TWmfRGB));
		m_pRGB[0] = c_oColorRed;
	}
	~CWmfColor()
	{
		if ( NULL != m_pRGB )
			free( m_pRGB );
	}

	void SetErrorHandler(EWmfError *pError)
	{
		m_pError = pError;
	}

	void Add(TWmfRGB *pRGB)
	{
		if ( CheckError() )
			return;

		bool bNewColor = true;

		for (unsigned long ulIndex = 0; ulIndex < m_ulCount; ulIndex++)
		{	
			if ( ( pRGB->r == m_pRGB[ulIndex].r ) && ( pRGB->g == m_pRGB[ulIndex].g ) && ( pRGB->b == m_pRGB[ulIndex].b ) )
			{	
				bNewColor = false;
				break;
			}
		}

		if ( !bNewColor ) 
			return;

        if ( m_ulCount == m_ulmax )
		{	
            TWmfRGB *pData = (TWmfRGB *)realloc( m_pRGB, (m_ulmax + 32) * sizeof(TWmfRGB) );
			if ( NULL == pData )
			{
				SetError( wmf_error_NotEnoughMemory );
				return;
			}

			m_pRGB   = pData;
            m_ulmax += 32;
		}

		m_pRGB[m_ulCount] = (*pRGB);
		m_ulCount++;
	}



	unsigned long GetColorIndex(TWmfRGB *pRGB)
	{
		unsigned long ulBest = 0;

		unsigned int unDistBest = 766;

		if ( CheckError() || 0 == m_ulCount )
		{
			return ulBest;
		}

		// Сначала ищем точное совпадение
		for (unsigned long ulIndex = 0; ulIndex < m_ulCount; ulIndex++)
		{	
			if ( ( pRGB->r == m_pRGB[ulIndex].r ) && ( pRGB->g == m_pRGB[ulIndex].g ) && ( pRGB->b == m_pRGB[ulIndex].b ) )
			{
				ulBest = ulIndex;
				unDistBest = 0;
				break;
			}
		}

		if ( 0 == unDistBest ) 
			return ulBest;

		// Ищем наиболее близкий цвет
		for (unsigned long ulIndex = 0; ulIndex < m_ulCount; ulIndex++)
		{	
			unsigned int unDistR = (unsigned int) abs(((int) (pRGB->r)) - ((int) (m_pRGB[ulIndex].r)));
			unsigned int unDistG = (unsigned int) abs(((int) (pRGB->g)) - ((int) (m_pRGB[ulIndex].g)));
			unsigned int unDistB = (unsigned int) abs(((int) (pRGB->b)) - ((int) (m_pRGB[ulIndex].b)));

            unsigned int unDist = (std::max)( (std::max)( unDistR, unDistG ),unDistB );

			if ( unDist < unDistBest )
			{
				ulBest = ulIndex;
				unDistBest = unDist;
			}
		}

		return ulBest;
	}

	unsigned long GetCount()
	{	
		return m_ulCount;
	}

	TWmfRGB* GetColor(unsigned long ulIndex)
	{
		if ( CheckError() || ulIndex >= m_ulCount )
			return ((TWmfRGB *)&c_oColorRed);

		return (m_pRGB + ulIndex);
	}

public:

	static TWmfRGB LongToRGB(unsigned short ushFirstByte, unsigned short ushSecondByte)
	{
		TWmfRGB oColor;

		oColor.r = (unsigned char) (ushFirstByte & 0x00FF);
		oColor.g = (unsigned char)((ushFirstByte & 0xFF00) >> 8);
		oColor.b = (unsigned char) (ushSecondByte & 0x00FF);

		return oColor;
	}

	static TWmfRGB WhiteColor()
	{
		return c_oColorWhite;
	}
	static TWmfRGB BlackColor()
	{
		return c_oColorWhite;
	}

	static TWmfRGB RGBColor(float fRed, float fGreen, float fBlue)
	{	
		TWmfRGB oColor;

        fRed   = (std::max)( 0.0f, (std::min)( 1.0f, fRed   ) );
        fGreen = (std::max)( 0.0f, (std::min)( 1.0f, fGreen ) );
        fBlue  = (std::max)( 0.0f, (std::min)( 1.0f, fBlue  ) );

		int nRed   = (int) (fRed   * (float) 256);
		int nGreen = (int) (fGreen * (float) 256);
		int nBlue  = (int) (fBlue  * (float) 256);

        nRed   = (std::max)( 0, (std::min)( 255, nRed   ) );
        nGreen = (std::max)( 0, (std::min)( 255, nGreen ) );
        nBlue  = (std::max)( 0, (std::min)( 255, nBlue  ) );

		oColor.r = (unsigned char) nRed;
		oColor.g = (unsigned char) nGreen;
		oColor.b = (unsigned char) nBlue;

		return oColor;
	}

private:

	bool CheckError()
	{
		if ( NULL == m_pError || wmf_error_None == *m_pError )
			return false;

		return true;
	}
	void SetError(EWmfError eError)
	{
		if ( NULL != m_pError )
		{
			*m_pError = eError;
		}
	}

private:

    unsigned long m_ulmax;
	unsigned long m_ulCount;

	TWmfRGB      *m_pRGB;

	EWmfError    *m_pError;

};

#endif /* _WMF_COLOR_H */
