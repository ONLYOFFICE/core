#pragma once

namespace DocFileFormat
{
  class TabDescriptor
  {
    public:
	  /// Justification code:
      /// 0 left tab
      /// 1 centered tab
      /// 2 right tab
      /// 3 decimal tab
      /// 4 bar
      unsigned char jc;

      /// Tab leader code:
      /// 0 no leader
      /// 1 dotted leader
      /// 2 hyphenated leader
      /// 3 single line leader
      /// 4 heavy line leader
      /// 5 middle dot
      unsigned char tlc;

      /// Parses the bytes to retrieve a TabDescriptor
	  TabDescriptor( unsigned char b ):
      jc(0), tlc(0)
      {
	    this->jc = b & 0x07;
        this->tlc = b >> 3;
      }
  };
}