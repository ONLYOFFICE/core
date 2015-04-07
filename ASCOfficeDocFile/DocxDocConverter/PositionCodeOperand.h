#pragma once

namespace ASCDocFileFormat
{
  union PositionCodeOperand
  {
    private:
	  struct
	  {
	    unsigned char padding:4;
		unsigned char pcVert:2;
		unsigned char pcHorz:2;
	  } PositionCodeOperandStruct;

	  unsigned char PositionCodeOperandByte;

    public:
	  PositionCodeOperand():
      PositionCodeOperandByte(0)
	  {
	  }

	  explicit PositionCodeOperand( unsigned char _pcVert, unsigned char _pcHorz ):
      PositionCodeOperandByte(0)
	  {
	    this->PositionCodeOperandStruct.padding = 0;
		this->PositionCodeOperandStruct.pcVert = _pcVert;
	    this->PositionCodeOperandStruct.pcHorz = _pcHorz;
	  }

	  explicit PositionCodeOperand( unsigned char _positionCodeOperand ):
      PositionCodeOperandByte(_positionCodeOperand)
	  {
	  }

	  operator unsigned char() const
	  {
	    return this->PositionCodeOperandByte;
	  }
  };
}