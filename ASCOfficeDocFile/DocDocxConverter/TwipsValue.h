#pragma once

namespace DocFileFormat
{
	class TwipsValue
	{
	private: 
		double value;

		/// The dots per inch value that should be used.
	public: 

		static const unsigned char Dpi = 72;

		/// Creates a new TwipsValue for the given value.
		TwipsValue( double _value = 0 ): value(_value)
		{
		}

		TwipsValue( const TwipsValue& _twipsValue )
		{
			value = _twipsValue.value;
		}

		TwipsValue& operator = ( const TwipsValue& _twipsValue )
		{
			if ( this != &_twipsValue )
				value = _twipsValue.value;

			return *this;
		}

		/// Converts the twips to pt
		inline double ToPoints() const
		{
			return value / 20.0;
		}

		/// Converts the twips to inch
		inline double ToInch() const
		{
			return value / ( Dpi * 20.0 );
		}

		/// Converts the twips to mm
		inline double ToMm() const
		{
			return ToInch() * 25.399931;
		}

		/// Converts the twips to cm
		inline double ToCm() const
		{
			return ToMm() / 10.0;
		}
	};
}