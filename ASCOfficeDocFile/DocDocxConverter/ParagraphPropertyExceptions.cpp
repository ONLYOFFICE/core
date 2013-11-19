#include "stdafx.h"
#include "ParagraphPropertyExceptions.h"

namespace DocFileFormat
{
	ParagraphPropertyExceptions::ParagraphPropertyExceptions( byte* bytes, int size, IStream* dataStream ): 
    PropertyExceptions( ( bytes + 2 ), ( size - 2 ) )
    {
      if ( size != 0 )
      {
	    this->istd = FormatUtils::BytesToUInt16( bytes, 0, size );
      }

	  VirtualStreamReader *reader = NULL;

      //There is a SPRM that points to an offset in the data stream, 
      //where a list of SPRM is saved.
      for ( list<SinglePropertyModifier>::iterator iter = this->grpprl->begin(); iter != this->grpprl->end(); iter++ )
      {
	    SinglePropertyModifier sprm( *iter );
		  
		if( ( sprm.OpCode == sprmPHugePapx ) || ( (int)sprm.OpCode == 0x6646 ) )
        {
          unsigned int fc = FormatUtils::BytesToUInt32( sprm.Arguments, 0, sprm.argumentsSize );
		  reader = new VirtualStreamReader( dataStream, (int)fc );
		  
          //parse the size of the external grpprl
		  byte* sizebytes = reader->ReadBytes( 2, true );
		  
		  unsigned int grpprlsize = FormatUtils::BytesToUInt16( sizebytes, 0, 2 );
          
		  RELEASEARRAYOBJECTS( sizebytes );

          //parse the external grpprl
		  byte* grpprlBytes = reader->ReadBytes( grpprlsize, true );

		  PropertyExceptions externalPx( grpprlBytes, grpprlsize );

          //assign the external grpprl
          RELEASEOBJECT( this->grpprl );
		  this->grpprl = new list<SinglePropertyModifier>( *(externalPx.grpprl) );

          //remove the sprmPHugePapx
		  this->grpprl->remove( sprm );

		  RELEASEARRAYOBJECTS( grpprlBytes );
		  RELEASEOBJECT( reader )

		  break;
		}
      }
    }
}
