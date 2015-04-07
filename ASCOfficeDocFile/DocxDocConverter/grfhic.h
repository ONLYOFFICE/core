#pragma once

namespace ASCDocFileFormat
{
  union grfhic
  {
    private:
	  struct
	  {
	    unsigned char fhicChecked:1;
		unsigned char fhicFormat:1;
		unsigned char fhicListText:1;
		unsigned char fhicPeriod:1;
		unsigned char fhicLeft1:1;
		unsigned char fhicListTab:1;
		unsigned char unused:1;
		unsigned char fhicBullet:1;
	  } grfhicStruct;

	  unsigned char grfhicByte;

    public:
	  grfhic():
      grfhicByte(0)
	  {
	  }

	  explicit grfhic( bool _fhicChecked, bool _fhicFormat, bool _fhicListText, bool _fhicPeriod,
		      bool _fhicLeft1, bool _fhicListTab, bool _fhicBullet ):
      grfhicByte(0)
	  {
	    ( _fhicChecked ) ? ( this->grfhicStruct.fhicChecked = 1 ) : ( this->grfhicStruct.fhicChecked = 0 );
		( _fhicFormat ) ? ( this->grfhicStruct.fhicFormat = 1 ) : ( this->grfhicStruct.fhicFormat = 0 );
		( _fhicListText ) ? ( this->grfhicStruct.fhicListText = 1 ) : ( this->grfhicStruct.fhicListText = 0 );
		( _fhicPeriod ) ? ( this->grfhicStruct.fhicPeriod = 1 ) : ( this->grfhicStruct.fhicPeriod = 0 );
		( _fhicLeft1 ) ? ( this->grfhicStruct.fhicLeft1 = 1 ) : ( this->grfhicStruct.fhicLeft1 = 0 );
		( _fhicListTab ) ? ( this->grfhicStruct.fhicListTab = 1 ) : ( this->grfhicStruct.fhicListTab = 0 );
		this->grfhicStruct.unused = 0;
	    ( _fhicBullet ) ? ( this->grfhicStruct.fhicBullet = 1 ) : ( this->grfhicStruct.fhicBullet = 0 );
	  }

	  explicit grfhic( unsigned char _grfhic ):
      grfhicByte(_grfhic)
	  {
	  }

	  operator unsigned char() const
	  {
	    return this->grfhicByte;
	  }
  };
}