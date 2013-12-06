#pragma once

namespace ASCDocFileFormat
{
  union grfhic
  {
    private:
	  struct
	  {
	    byte fhicChecked:1;
		byte fhicFormat:1;
		byte fhicListText:1;
		byte fhicPeriod:1;
		byte fhicLeft1:1;
		byte fhicListTab:1;
		byte unused:1;
		byte fhicBullet:1;
	  } grfhicStruct;

	  byte grfhicByte;

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

	  explicit grfhic( byte _grfhic ):
      grfhicByte(_grfhic)
	  {
	  }

	  operator byte() const
	  {
	    return this->grfhicByte;
	  }
  };
}