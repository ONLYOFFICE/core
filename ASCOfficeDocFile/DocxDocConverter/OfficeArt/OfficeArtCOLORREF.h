#pragma once

#include "../Common/FormatUtils.h"

namespace OfficeArt
{
	class OfficeArtCOLORREF
	{
	public:
		OfficeArtCOLORREF()
		{
			AVSDocFormatUtils::BitSet oShapeSettings (4);
			
			oShapeSettings.SetBit (0,0);	 
			oShapeSettings.SetBit (0,1);	
			oShapeSettings.SetBit (0,2);
			oShapeSettings.SetBit (0,3);	 
			oShapeSettings.SetBit (0,4);	
		}

		inline unsigned long Get()
		{
			return 0x00FFFFFF;
		}

	private:

		unsigned char	red; 
		unsigned char	green; 
		unsigned char	blue; 

		bool			fPaletteIndex;
		bool			fPaletteRGB;
		bool			fSystemRGB;
		bool			fSchemeIndex;
		bool			fSysIndex;

		unsigned char Index; // if SchemeIndex==0x01  A value of 0x1 specifies that red will be treated as an index into the current color scheme table. If this value is 0x1, green and blue MUST be 0x00
	};
}