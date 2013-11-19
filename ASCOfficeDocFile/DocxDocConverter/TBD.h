#pragma once

#include "Constants.h"

namespace AVSDocFileFormat
{
  union TBD
  {
    private:
	  struct
	  {
	    byte jc:3;
		byte tlc:3;
		byte UNUSED:2;
	  } TBDStruct;
	  byte TBDByte;

    public:
	  explicit TBD( byte _tbd = 0 ):
      TBDByte(_tbd)
	  {
	  }

	  TBD( Constants::TabJC _jc, Constants::TabLC _tlc ):
      TBDByte(0)
	  {
        this->TBDStruct.jc = (byte)_jc;
		this->TBDStruct.tlc = (byte)_tlc;
	  }

	  operator byte () const
	  {
	    return this->TBDByte;
	  }
  };
}