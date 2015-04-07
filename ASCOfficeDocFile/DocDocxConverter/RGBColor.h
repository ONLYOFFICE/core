#pragma once

#include "../../DesktopEditor/common/Types.h"

#if defined(_WIN32) || defined(_WIN64)
    #include <atlbase.h>
    #include <atlstr.h>
#else
    #include "../../DesktopEditor/common/ASCVariant.h"
    #include "../../Common/DocxFormat/Source/Base/ASCString.h"
#endif

#include "../../Common/DocxFormat/Source/XML/stringcommon.h"

namespace DocFileFormat
{
	typedef enum _ByteOrder 
	{
		RedFirst,
		RedLast
	} ByteOrder;

	class RGBColor
	{
	private:
		unsigned char Red;
		unsigned char Green;
		unsigned char Blue;
		unsigned char Alpha;

	public:
		wstring SixDigitHexCode;
		wstring EightDigitHexCode;

		RGBColor( int cv, ByteOrder order )
		{
			unsigned char* bytes = FormatUtils::GetBytes( cv );

//			wchar_t rgbColor6[7];
//			wchar_t rgbColor8[9];
            CString rgbColor6;
            CString rgbColor8;

			if( order == RedFirst )
			{
				//R
				this->Red = bytes[0];
				//G
				this->Green = bytes[1];
				//B
				this->Blue = bytes[2];
				//Alpha
				this->Alpha = bytes[3];

                rgbColor6.Format( _T( "%02x%02x%02x" ),     /*R*/this->Red, /*G*/this->Green, /*B*/this->Blue );
                rgbColor8.Format( _T( "%02x%02x%02x%02x" ), /*R*/this->Red, /*G*/this->Green, /*B*/this->Blue, /*A*/this->Alpha );

                SixDigitHexCode     = string2std_string( rgbColor6 );
                EightDigitHexCode   = string2std_string( rgbColor8 );
			}
			else if ( order == RedLast )
			{
				//R
				this->Red = bytes[2];
				//G
				this->Green = bytes[1];
				//B
				this->Blue = bytes[0];
				//Alpha
				this->Alpha = bytes[3];

                rgbColor6.Format( _T( "%02x%02x%02x" ),     /*R*/this->Red, /*G*/this->Green, /*B*/this->Blue );
                rgbColor8.Format( _T( "%02x%02x%02x%02x" ), /*R*/this->Red, /*G*/this->Green, /*B*/this->Blue, /*A*/this->Alpha );

                SixDigitHexCode     = string2std_string( rgbColor6 );
                EightDigitHexCode   = string2std_string( rgbColor8 );
            }

			RELEASEARRAYOBJECTS( bytes );
		}
	};
}
