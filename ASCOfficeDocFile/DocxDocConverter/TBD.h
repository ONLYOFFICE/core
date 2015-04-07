#pragma once

#include "Constants.h"

namespace ASCDocFileFormat
{
  union TBD
  {
    private:
	  struct
	  {
	    unsigned char jc:3;
		unsigned char tlc:3;
		unsigned char UNUSED:2;
	  } TBDStruct;
	  unsigned char TBDByte;

    public:
	  explicit TBD( unsigned char _tbd = 0 ):
      TBDByte(_tbd)
	  {
	  }

	  TBD( Constants::TabJC _jc, Constants::TabLC _tlc ):
      TBDByte(0)
	  {
        this->TBDStruct.jc = (unsigned char)_jc;
		this->TBDStruct.tlc = (unsigned char)_tlc;
	  }

	  operator unsigned char () const
	  {
	    return this->TBDByte;
	  }
  };
}