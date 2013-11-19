#pragma once

#include "VirtualStreamReader.h"
#include "ListLevel.h"

namespace DocFileFormat
{
  class ListFormatOverrideLevel
  {
    friend class NumberingMapping;
  
    private: 
	  /// Start-at value if fFormatting==false and fStartAt==true. 
      /// If fFormatting == true, the start is stored in the LVL
      int iStartAt;
      /// The level to be overridden
      byte ilvl;
      /// True if the start-at value is overridden
      bool fStartAt;
      /// True if the formatting is overridden
      bool fFormatting;
	  /// A grfhic that specifies HTML incompatibilities of the level.
      byte grfhic;
	  ListLevel* lvl;


    public:
	  ListFormatOverrideLevel():
	  iStartAt(0), ilvl(0), fStartAt(false), fFormatting(false), grfhic(0), lvl(NULL)
	  {
	  }

      virtual ~ListFormatOverrideLevel()
	  {
	    RELEASEOBJECT( this->lvl );
	  }
 
      /// Parses the bytes to retrieve a ListFormatOverrideLevel
      ListFormatOverrideLevel( VirtualStreamReader* reader, int length ):
	  iStartAt(0), ilvl(0), fStartAt(false), fFormatting(false), grfhic(0), lvl(NULL)
      {
	    this->iStartAt = reader->ReadInt32();
		unsigned int flag = (int)reader->ReadUInt32();
        this->ilvl = (byte)( flag & 0x000F );
		this->fStartAt = FormatUtils::BitmaskToBool( flag, 0x0010 );
		this->fFormatting = FormatUtils::BitmaskToBool( flag, 0x0020 );
		this->grfhic = FormatUtils::GetIntFromBits( flag, 6, 8 );
		
        //it's a complete override, so the fix part is followed by LVL struct
        if ( this->fFormatting )
        {
		  this->lvl = new ListLevel( reader, length );
        }
      }
  };
}