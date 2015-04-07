#pragma once

namespace ASCDocFileFormat
{
	union Brc80
	{
	private:
		struct
		{
			unsigned int dptLineWidth:8;
			unsigned int brcType:8;
			unsigned int ico:8;
			unsigned int dptSpace:5;
			unsigned int fShadow:1;
			unsigned int fFrame:1;
			unsigned int reserved:1;
		} Brc80Struct;
		unsigned int Brc80Int;

	public:
		Brc80() : Brc80Int(0xFFFFFFFF)
		{
		}

		explicit Brc80( unsigned int _brc80 ) : Brc80Int(_brc80)
		{
		}

		explicit Brc80( unsigned char _dptLineWidth, unsigned char _brcType, unsigned char _ico, unsigned char _dptSpace, bool fShadow, bool fFrame )
		{
			if ( _dptLineWidth < 2 )
			{
				this->Brc80Struct.dptLineWidth = 2;
			}
			else
			{
				this->Brc80Struct.dptLineWidth = _dptLineWidth;
			}

			if ( _brcType == 0x1A )
			{
				this->Brc80Struct.brcType = 0x18;
			}
			else if ( _brcType == 0x1B )
			{
				this->Brc80Struct.brcType = 0x19;
			}
			else
			{
				this->Brc80Struct.brcType = _brcType;
			}

			if ( _ico >= 0x11 )
			{
				this->Brc80Struct.ico = 0x10;
			}
			else
			{
				this->Brc80Struct.ico = _ico;
			}

			this->Brc80Struct.dptSpace = _dptSpace;

			( fShadow ) ? ( this->Brc80Struct.fShadow = 1 ) : ( this->Brc80Struct.fShadow = 0 );
			( fFrame ) ? ( this->Brc80Struct.fFrame = 1 ) : ( this->Brc80Struct.fFrame = 0 );

			this->Brc80Struct.reserved = 0;
		}

		operator unsigned int() const
		{
			return Brc80Int;
		}
	};

	class Brc80MayBeNil
	{
	private:
		Brc80 brc80;

	public:
		Brc80MayBeNil():
		  brc80()
		  {
		  }

		  explicit Brc80MayBeNil( unsigned int _brc80MayBeNil ):
		  brc80(_brc80MayBeNil)
		  {
		  }

		  Brc80MayBeNil( unsigned char _dptLineWidth, unsigned char _brcType, unsigned char _ico, unsigned char _dptSpace, bool fShadow, bool fFrame ):  brc80( _dptLineWidth, _brcType, _ico, _dptSpace, fShadow, fFrame )
		  {

		  }

		  operator unsigned int() const
		  {
			  return this->brc80;
		  }
	};
}