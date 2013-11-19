#pragma once

namespace OfficeArt
{
	enum COMPRESSION_METHOD
	{
		COMPRESSION_METHOD_DEFLATE = 0x00,
		COMPRESSION_METHOD_NO_COMPRESSION = 0xFE
	};

	struct POINT
	{
		int x;
		int y;

		POINT():
		x(0), y(0)
		{
		}

		explicit POINT( int _x, int _y ):
		x(_x), y(_y)
		{
		}
	};

	struct RECT
	{
		int left;
		int top;
		int right;
		int bottom;

		RECT():
		left(0), top(0), right(0), bottom(0)
		{
		}

		explicit RECT( int _left, int _top, int _right, int _bottom ):
		left(_left), top(_top), right(_right), bottom(_bottom)
		{
		}
	};

	class EnglishMetricUnit
	{
	private:
		long value;

	public:
		explicit EnglishMetricUnit( long _value = 0 ):
		value(_value)
		{
		}

		void FromTwip( long _value )
		{
			this->value = ( _value * 65 );
		}

		void FromHalfPoint( long _value )
		{
			this->value = ( _value * 6350 );
		}

		void FromPoint( long _value )
		{
			this->value = ( _value * 12700 );
		}

		void FromInch( long _value )
		{
			this->value = ( _value * 914400 );
		}

		long GetValue() const
		{
			return this->value;
		}

		long ToTwip() const
		{
			return ( this->value / 65 );
		}

		long ToHalfPoint() const
		{
			return ( this->value / 6350 );
		}

		long ToPoint() const
		{
			return ( this->value / 12700 );
		}

		long ToInch() const
		{
			return ( this->value / 914400 );
		}
	};

	typedef unsigned int MSOSPID;
}

namespace OfficeArt
{
	struct ADJH
	{
		int handle[9];
	};

	struct SG
	{
		int sg[2];
	};

}