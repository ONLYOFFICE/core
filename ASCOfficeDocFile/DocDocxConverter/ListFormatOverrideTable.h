#pragma once

#include "VirtualStreamReader.h"
#include "FileInformationBlock.h"
#include "ListFormatOverride.h"

namespace DocFileFormat
{
  class ListFormatOverrideTable: public vector<ListFormatOverride*>
  {
    private:
	  static const int LFO_LENGTH = 16;
      static const int LFOLVL_LENGTH = 6;
	  vector<unsigned int> cps;

    public:
	  ListFormatOverrideTable( FileInformationBlock* fib, POLE::Stream* tableStream )
      {
	    if ( fib->m_FibWord97.lcbPlfLfo > 0 )
        {
	      VirtualStreamReader reader( tableStream, fib->m_FibWord97.fcPlfLfo );
            
          //read the count of LFOs
          int count = reader.ReadInt32();

          //read the LFOs
          for ( int i = 0; i < count; i++ )
          {
	    	this->push_back( new ListFormatOverride( &reader, LFO_LENGTH ) );
          }

          //read the LFOLVLs
          for ( int i = 0; i < count; i++ )
          {
		    this->cps.push_back( reader.ReadUInt32() );
			
			for ( int j = 0; j < this->at( i )->clfolvl; j++ )
            {
			  this->at( i )->rgLfoLvl[j] = new ListFormatOverrideLevel( &reader, LFOLVL_LENGTH );
            }
          }
        }
      }

	  virtual ~ListFormatOverrideTable()
	  {
	    for ( vector<ListFormatOverride*>::iterator iter = this->begin(); iter != this->end(); iter++ )
		{
		  RELEASEOBJECT( *iter );
		}
	  }
  };
}