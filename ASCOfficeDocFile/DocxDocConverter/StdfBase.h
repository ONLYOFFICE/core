#pragma once

#include "GRFSTD.h"
#include "Constants.h"

namespace ASCDocFileFormat
{
  union StdfBase
  {
    private:
	  static const byte SIZE_IN_BYTES = 10;

	  struct
	  {
	    unsigned short sti:12;
		unsigned short fScratch:1;
		unsigned short fInvalHeight:1;
		unsigned short fHasUpe:1;
		unsigned short fMassCopy:1;
		unsigned short stk:4;
		unsigned short istdBase:12;
		unsigned short cupx:4;
		unsigned short istdNext:12;
		unsigned short bchUpe:16;
		unsigned short grfstd:16;
	  } StdfBaseStruct;
	  byte StdfBaseBytes[SIZE_IN_BYTES];

    public:
	  StdfBase()
	  {
	    memset( this->StdfBaseBytes, 0, SIZE_IN_BYTES );
	  }

	  explicit StdfBase( unsigned short _sti, Constants::StyleType _stk, unsigned short _istdBase, 
		        unsigned short _istdNext, const GRFSTD& _grfstd )
	  {
	    memset( this->StdfBaseBytes, 0, SIZE_IN_BYTES );

	    if ( _sti > 0x0FFE )
	    {
	      this->StdfBaseStruct.sti = 0x0FFE;
	    }
	    else
	    {
	      this->StdfBaseStruct.sti = _sti;
	    }

	    this->StdfBaseStruct.stk = (unsigned short)_stk;

	    if ( _istdBase > 0x0FFF )
	    {
	      this->StdfBaseStruct.istdBase = 0x0FFF;
	    }
	    else
	    {
	      this->StdfBaseStruct.istdBase = _istdBase;
	    }

	    //!!!TODO: cupx for revision-marked-style!!!
	    switch ( _stk )
	    {
	      case Constants::styleTypeParagraph:
		  {
		    this->StdfBaseStruct.cupx = 2;
		  }
		  break;

		  case Constants::styleTypeCharacter:
		  {
		    this->StdfBaseStruct.cupx = 1;
		  }
		  break;

		  case Constants::styleTypeTable:
		  {
		    this->StdfBaseStruct.cupx = 3;
		  }
		  break;

		  case Constants::styleTypeNumbering:
		  {
		    this->StdfBaseStruct.cupx = 1;
	 	  }
	      break;
	    }

	    if ( _istdNext > 0x0FFF )
	    {
	      this->StdfBaseStruct.istdNext = 0x0FFF;
	    }
	    else
	    {
	      this->StdfBaseStruct.istdNext = _istdNext;
	    }

	    this->StdfBaseStruct.grfstd = (unsigned short)_grfstd;
	  }

	  StdfBase( const StdfBase& _stdfBase )
	  {
	    memset( this->StdfBaseBytes, 0, SIZE_IN_BYTES );
		memcpy( this->StdfBaseBytes, _stdfBase.StdfBaseBytes, SIZE_IN_BYTES );
	  }

	  operator byte*() const
	  {
	    return (byte*)(this->StdfBaseBytes);
	  }
  };
}