#pragma once

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
		byte Red;
		byte Green;
		byte Blue;
		byte Alpha;

	public:
		wstring SixDigitHexCode;
		wstring EightDigitHexCode;

		RGBColor( int cv, ByteOrder order )
		{
			byte* bytes = FormatUtils::GetBytes( cv );

			WCHAR rgbColor6[7];
			WCHAR rgbColor8[9];

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

				swprintf_s( rgbColor6, _T( "%02x%02x%02x" ), /*R*/this->Red, /*G*/this->Green, /*B*/this->Blue );
				swprintf_s( rgbColor8, _T( "%02x%02x%02x%02x" ), /*R*/this->Red, /*G*/this->Green, /*B*/this->Blue, /*A*/this->Alpha );

				SixDigitHexCode = wstring( rgbColor6 );
				EightDigitHexCode = wstring( rgbColor8 );
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

				swprintf_s( rgbColor6, _T( "%02x%02x%02x" ), /*R*/this->Red, /*G*/this->Green, /*B*/this->Blue );
				swprintf_s( rgbColor8, _T( "%02x%02x%02x%02x" ), /*R*/this->Red, /*G*/this->Green, /*B*/this->Blue, /*A*/this->Alpha );

				SixDigitHexCode = wstring( rgbColor6 );
				EightDigitHexCode = wstring( rgbColor8 );
			}

			RELEASEARRAYOBJECTS( bytes );
		}
	};
}