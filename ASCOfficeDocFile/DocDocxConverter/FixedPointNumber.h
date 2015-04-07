#pragma once

namespace DocFileFormat
{
	/// Specifies an approximation of a real number, where the approximation has a fixed number of digits after the radix point. 
	/// 
	/// This type is specified in [MS-OSHARED] section 2.2.1.6.
	/// 
	/// Value of the real number = Integral + ( Fractional / 65536.0 ) 
	/// 
	/// Integral (2 bytes): A signed integer that specifies the integral part of the real number. 
	/// Fractional (2 bytes): An unsigned integer that specifies the fractional part of the real number.
	class FixedPointNumber
	{ 
	public:
		unsigned short Integral;
		unsigned short Fractional;

		FixedPointNumber( unsigned short integral = 0, unsigned short fractional = 0 )
		{
			this->Integral = integral;
			this->Fractional = fractional;
		}

		FixedPointNumber( unsigned int value )
		{
			unsigned char* bytes = FormatUtils::GetBytes( value );

			this->Integral = FormatUtils::BytesToUInt16( bytes, 0, sizeof(value) );
			this->Fractional = FormatUtils::BytesToUInt16( bytes, 2, sizeof(value) );

			RELEASEARRAYOBJECTS( bytes );
		}

		FixedPointNumber( const unsigned char* bytes, unsigned int size )
		{
			if ( ( bytes != NULL ) && ( size >= 4 ) )
			{
				this->Integral = FormatUtils::BytesToUInt16( bytes, 0, size );
				this->Fractional = FormatUtils::BytesToUInt16( bytes, 2, size );
			}
		}

		double ToAngle() const
		{
			if ( this->Fractional != 0 )
			{
				// negative angle
				return ( this->Fractional - 65536.0 );
			}
			else if ( this->Integral != 0 )
			{
				//positive angle
				return ( 65536.0 - this->Integral );
			}
			else
			{
				return 0.0;
			}
		}

		double GetValue() const
		{
			return (double)( this->Integral + ( (double)this->Fractional / 65536.0 ) );
		}
	};
}