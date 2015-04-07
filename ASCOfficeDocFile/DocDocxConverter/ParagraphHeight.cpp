
#include "ParagraphHeight.h"
#include "../Common/FormatUtils.h"

namespace DocFileFormat
{
	ParagraphHeight::ParagraphHeight()
    {
      //set default values
      setDefaultValues();
    }

    /*========================================================================================================*/

	ParagraphHeight::ParagraphHeight( unsigned char* bytes, int size, bool fTtpMode )
    {
      //set default values
      setDefaultValues();

      if ( size == 12 )
      {
        // The ParagraphHeight is placed in a ParagraphProperties whose fTtp field is set, 
        //so used another bit setting
        if ( fTtpMode )
        {
		  this->fSpare = FormatUtils::BitmaskToBool( FormatUtils::BytesToInt16( bytes, 0, size ), 0x0001 );
		  this->fUnk = FormatUtils::BitmaskToBool( FormatUtils::BytesToInt16( bytes, 0, size ), 0x0002 );
		  this->dcpTtpNext = FormatUtils::BytesToInt16( bytes, 0, size );
		  this->dxaCol = FormatUtils::BytesToInt32( bytes, 4, size );
		  this->dymTableHeight = FormatUtils::BytesToInt32( bytes, 8, size );
        }
        else
        {
		  this->fVolatile = FormatUtils::BitmaskToBool( FormatUtils::BytesToInt16( bytes, 0, size ), 0x0001 );
          this->fUnk = FormatUtils::BitmaskToBool( FormatUtils::BytesToInt16( bytes, 0, size ), 0x0002 );
          this->fDiffLines = FormatUtils::BitmaskToBool( FormatUtils::BytesToInt16( bytes, 0, size ), 0x0004 );
          this->clMac = FormatUtils::BytesToUInt16( bytes, 0, size ) & 0x00FF;

          this->dxaCol = FormatUtils::BytesToInt32( bytes, 4, size );
          this->dymLine = FormatUtils::BytesToInt32( bytes, 8, size );
          this->dymHeight = FormatUtils::BytesToInt32( bytes, 8, size );
        }
      }
      else
      {
        //throw new ByteParseException("Cannot parse the struct ParagraphHeight, the length of the struct doesn't match");
      }
    }

	/*========================================================================================================*/

	void ParagraphHeight::setDefaultValues()
    {
      this->clMac = 0;
      this->dcpTtpNext = 0;
      this->dxaCol = 0;
      this->dymHeight = 0;
      this->dymLine = 0;
      this->dymTableHeight = 0;
      this->fDiffLines = false;
      this->fSpare = false;
      this->fUnk = false;
      this->fVolatile = false;
    }
}