#pragma once

namespace AVSDocFileFormat
{
  union PositionCodeOperand
  {
    private:
	  struct
	  {
	    byte padding:4;
		byte pcVert:2;
		byte pcHorz:2;
	  } PositionCodeOperandStruct;

	  byte PositionCodeOperandByte;

    public:
	  PositionCodeOperand():
      PositionCodeOperandByte(0)
	  {
	  }

	  explicit PositionCodeOperand( byte _pcVert, byte _pcHorz ):
      PositionCodeOperandByte(0)
	  {
	    this->PositionCodeOperandStruct.padding = 0;
		this->PositionCodeOperandStruct.pcVert = _pcVert;
	    this->PositionCodeOperandStruct.pcHorz = _pcHorz;
	  }

	  explicit PositionCodeOperand( byte _positionCodeOperand ):
      PositionCodeOperandByte(_positionCodeOperand)
	  {
	  }

	  operator byte() const
	  {
	    return this->PositionCodeOperandByte;
	  }
  };
}