#pragma once

namespace ASCDocFileFormat
{
  union GRFSTD
  {
    private:
	  struct
	  {
	    unsigned short fAutoRedef:1;
		unsigned short fHidden:1;
		unsigned short f97LidsSet:1;
        unsigned short fCopyLang:1;
        unsigned short fPersonalCompose:1;
        unsigned short fPersonalReply:1;
        unsigned short fPersonal:1;
        unsigned short fNoHtmlExport:1;
        unsigned short fSemiHidden:1;
        unsigned short fLocked:1;
        unsigned short fInternalUse:1;
        unsigned short fUnhideWhenUsed:1;
        unsigned short fQFormat:1;
        unsigned short fReserved:3;
	  } GRFSTDStruct;
	  unsigned short GRFSTDUnsignedShort;

    public:
	  GRFSTD():
      GRFSTDUnsignedShort(0)
	  {
	  }

	  explicit GRFSTD( unsigned short _grfstd ):
      GRFSTDUnsignedShort(_grfstd)
	  {
	  }

	  explicit GRFSTD( bool _fAutoRedef, bool _fHidden, bool _fPersonalCompose, bool _fPersonalReply, bool _fPersonal, 
		      bool _fSemiHidden, bool _fLocked, bool _fUnhideWhenUsed, bool _fQFormat ):
      GRFSTDUnsignedShort(0)
	  {
	    ( _fAutoRedef ) ? ( this->GRFSTDStruct.fAutoRedef = 1 ) : ( this->GRFSTDStruct.fAutoRedef = 0 );
		( _fHidden ) ? ( this->GRFSTDStruct.fHidden = 1 ) : ( this->GRFSTDStruct.fHidden = 0 );
		this->GRFSTDStruct.f97LidsSet = 0;
		this->GRFSTDStruct.fCopyLang = 0;
		( _fPersonalCompose ) ? ( this->GRFSTDStruct.fPersonalCompose = 1 ) : ( this->GRFSTDStruct.fPersonalCompose = 0 );
		( _fPersonalReply ) ? ( this->GRFSTDStruct.fPersonalReply = 1 ) : ( this->GRFSTDStruct.fPersonal = 0 );
		( _fPersonal ) ? ( this->GRFSTDStruct.fPersonal = 1 ) : ( this->GRFSTDStruct.fPersonal = 0 );
		this->GRFSTDStruct.fNoHtmlExport = 0;
		( _fSemiHidden ) ? ( this->GRFSTDStruct.fSemiHidden = 1 ) : ( this->GRFSTDStruct.fSemiHidden = 0 );
		( _fLocked ) ? ( this->GRFSTDStruct.fLocked = 1 ) : ( this->GRFSTDStruct.fLocked = 0 );
		this->GRFSTDStruct.fInternalUse = 0;
		( _fUnhideWhenUsed ) ? ( this->GRFSTDStruct.fUnhideWhenUsed = 1 ) : ( this->GRFSTDStruct.fUnhideWhenUsed = 0 );
		( _fQFormat ) ? ( this->GRFSTDStruct.fQFormat = 1 ) : ( this->GRFSTDStruct.fQFormat = 0 );
		this->GRFSTDStruct.fReserved = 0;
	  }

	  operator unsigned short() const
	  {
	    return this->GRFSTDUnsignedShort;
	  }
  };
}