#pragma once

#include "VirtualStreamReader.h"
#include "ListFormatOverrideLevel.h"

namespace DocFileFormat
{
  class ListFormatOverride
  {
    friend class ListFormatOverrideTable;
	friend class NumberingMapping;
  
    private: 
	  static const int LFO_LENGTH = 16;
	  /// List ID of corresponding ListData
      int lsid;
      /// Count of levels whose format is overridden
      byte clfolvl;
      /// Specifies the field this LFO represents. 
      /// MUST be a value from the following table:
      /// 0x00:   This LFO is not used for any field.
      /// 0xFC:   This LFO is used for the AUTONUMLGL field.
      /// 0xFD:   This LFO is used for the AUTONUMOUT field.
      /// 0xFE:   This LFO is used for the AUTONUM field.
      /// 0xFF:   This LFO is not used for any field.
      byte ibstFltAutoNum;
      /// A grfhic that specifies HTML incompatibilities.
      byte grfhic;
      /// Array of all levels whose format is overridden
      vector<ListFormatOverrideLevel*> rgLfoLvl;

    public:
	  /// Parses the given Stream Reader to retrieve a ListFormatOverride
	  ListFormatOverride( VirtualStreamReader* reader, int length ):
	  lsid(0), clfolvl(0), ibstFltAutoNum(0), grfhic(0)
      {
	    long startPos = reader->GetPosition();

        this->lsid = reader->ReadInt32();
        reader->ReadBytes( 8, false );
        this->clfolvl = reader->ReadByte();
        this->ibstFltAutoNum = reader->ReadByte();
        this->grfhic = reader->ReadByte();
        reader->ReadByte();

        if ( this->clfolvl != 0 )
		{
		  this->rgLfoLvl = vector<ListFormatOverrideLevel*>( this->clfolvl );
		}

		reader->Seek( startPos, STREAM_SEEK_SET );
        reader->ReadBytes( LFO_LENGTH, false );
      }

	  virtual ~ListFormatOverride()
	  {
	    for ( vector<ListFormatOverrideLevel*>::iterator iter = this->rgLfoLvl.begin(); iter != this->rgLfoLvl.end(); iter++ )
		{
		  RELEASEOBJECT( *iter );  
		}
	  }
  };
}