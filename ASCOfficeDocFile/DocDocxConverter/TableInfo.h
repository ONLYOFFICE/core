#pragma once

#include "ParagraphPropertyExceptions.h"

namespace DocFileFormat
{
  class TableInfo
  {
    public:
	  bool fInTable;
      bool fTtp;
      bool fInnerTtp;
      bool fInnerTableCell;
      unsigned int iTap;

      TableInfo( ParagraphPropertyExceptions* papx ):
      fInTable(false), fTtp(false), fInnerTtp(false), fInnerTableCell(false), iTap(0)
      {
	    if ( papx != NULL )
		{
		  for ( list<SinglePropertyModifier>::iterator iter = papx->grpprl->begin(); iter != papx->grpprl->end(); iter++ )
	      {
	        if ( iter->OpCode == sprmPFInTable )
            {
			  this->fInTable = ( iter->Arguments[0] == 1 ) ? (true) : (false);
            }
          
		    if ( iter->OpCode == sprmPFTtp )
            {
              this->fTtp = ( iter->Arguments[0] == 1 ) ? (true) : (false);
            }
          
		    if ( iter->OpCode == sprmPFInnerTableCell )
            {
              this->fInnerTableCell = ( iter->Arguments[0] == 1 ) ? (true) : (false);
            }
          
		    if ( iter->OpCode == sprmPFInnerTtp )
            {
              this->fInnerTtp = ( iter->Arguments[0] == 1 ) ? (true) : (false);
            }
          
		    if ( iter->OpCode == sprmPItap )
            {
		      this->iTap = FormatUtils::BytesToUInt32( iter->Arguments, 0, iter->argumentsSize );
          
			  if ( this->iTap > 0 )
			  {
			    this->fInTable = true;
			  }
            }
          
		    if ( (int)( iter->OpCode ) == 0x66A )
            {
              //add value!
              this->iTap = FormatUtils::BytesToUInt32( iter->Arguments, 0, iter->argumentsSize );

			  if ( this->iTap > 0 )
			  {
                this->fInTable = true;
			  }
            }
          }
		}
      }
  };
}