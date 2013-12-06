#pragma once

namespace ASCDocFileFormat
{
  union Shd80
  {
    private:
	  struct
	  {
	    byte icoFore:5;
		byte icoBack:5;
		byte ipat:6;
	  } Shd80Struct;
	  unsigned short Shd80UnsignedShort;

    public:
	  Shd80():
	  Shd80UnsignedShort(0)
	  {
	    Shd80Struct.icoFore = 0x1F;
		Shd80Struct.icoBack = 0x1F;
		Shd80Struct.ipat = 0x3F;
	  }

	  explicit Shd80( byte _icoFore, byte _icoBack, byte _ipat ):
	  Shd80UnsignedShort(0)
	  {
	    Shd80Struct.icoFore = _icoFore;
	    Shd80Struct.icoBack = _icoBack;
		Shd80Struct.ipat = _ipat;
	  }

      operator unsigned short() const
	  {
	    return this->Shd80UnsignedShort;
	  }
  };
}