#include "stdafx.h"
#include "PropertyExceptions.h"

namespace DocFileFormat
{
  PropertyExceptions::~PropertyExceptions()
  {
    RELEASEOBJECT( this->grpprl );
  }

  /*========================================================================================================*/

  PropertyExceptions::PropertyExceptions(): grpprl(NULL)
  {
    this->grpprl = new list<SinglePropertyModifier>();
  }

  /*========================================================================================================*/

  PropertyExceptions::PropertyExceptions( const list<SinglePropertyModifier>& grpprl )
  {
    this->grpprl = new list<SinglePropertyModifier>( grpprl );
  }

  /*========================================================================================================*/

  PropertyExceptions::PropertyExceptions( byte* bytes, int size ): grpprl(NULL)
  {
    this->grpprl = new list<SinglePropertyModifier>();

    if ( ( bytes != NULL ) && ( size != 0 ) )
    {
      //read the sprms
              
      int sprmStart = 0;
      bool goOn = true;
      
	  while ( goOn )
      {
        //enough bytes to read?
        if ( ( sprmStart + 2 ) < size )
        {
          //make spra
		  OperationCode opCode = (OperationCode)FormatUtils::BytesToUInt16( bytes, sprmStart, size );
          byte spra = (byte)( (int)opCode >> 13 );

          // get size of operand
		  short opSize = (short)SinglePropertyModifier::GetOperandSize( spra );
          byte lenByte = 0;

          //operand has variable size
          if ( opSize == 255 )
          {
            //some opCode need special treatment
            switch ( opCode )
            {
			  case sprmTDefTable:
			  case sprmTDefTable10:
              {
			    //The opSize of the table definition is stored in 2 bytes instead of 1
                lenByte = 2;
				opSize = FormatUtils::BytesToInt16( bytes, ( sprmStart + 2 ), size );
                //Word adds an additional byte to the opSize to compensate the additional
                //byte needed for the length
                opSize--;
			  }
              break;
              
			  case sprmPChgTabs:
			  {
			    //The tab operand can be bigger than 255 bytes (length byte is set to 255).
                //In this case a special calculation of the opSize is needed
                lenByte = 1;
                opSize = bytes[sprmStart + 2];
                
				if ( opSize == 255 )
                {
                  byte itbdDelMax = bytes[sprmStart + 3];
                  byte itbdAddMax = bytes[sprmStart + 3 + 2 * itbdDelMax];
                  opSize = (short)( ( itbdDelMax * 4 + itbdAddMax * 3 ) - 1 );
                }
			  }
              break;
              
			  default:
		      {  //The variable length stand in the byte after the opcode
                lenByte = 1;
                opSize = bytes[sprmStart + 2];
			  }
              break;
            }
          }

          //copy sprm to array
          //length is 2byte for the opCode, lenByte for the length, opSize for the length of the operand
          int sprmBytesSize = 2 + lenByte + opSize;
		  byte* sprmBytes = NULL;
		  
		  sprmBytes = new byte[sprmBytesSize];

          if ( size >= ( sprmStart + sprmBytesSize ) )
          {
            memcpy( sprmBytes, ( bytes + sprmStart ), sprmBytesSize );

            //parse
            SinglePropertyModifier sprm( sprmBytes, sprmBytesSize );
			grpprl->push_back( sprm );

            sprmStart += sprmBytesSize;
          }
          else
          {
            goOn = false;
          }

		  RELEASEARRAYOBJECTS( sprmBytes );
        }
        else
        {
          goOn = false;
        }
      }
    }
  }
}