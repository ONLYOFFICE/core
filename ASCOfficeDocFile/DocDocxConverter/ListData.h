#pragma once

#include "ListLevel.h"

namespace DocFileFormat
{
  class ListData
  {
    friend class ListTable;
	friend class NumberingMapping;

    private:
	  /// Unique List ID
      int lsid;
      /// Unique template code
      int tplc;
      /// Array of shorts containing the istd‘s linked to each level of the list, 
      /// or ISTD_NIL (4095) if no style is linked.
      vector<short> rgistd;
      /// True if this is a simple (one-level) list.
      /// False if this is a multilevel (nine-level) list.
      bool fSimpleList;
      /// Word 6.0 compatibility option:
      /// True if the list should start numbering over at the beginning of each section.
      bool fRestartHdn;
      /// To emulate Word 6.0 numbering:
      /// True if Auto numbering
      bool fAutoNum;
      /// When true, this list was there before we started reading RTF.
      bool fPreRTF;
      /// When true, list is a hybrid multilevel/simple (UI=simple, internal=multilevel)
      bool fHybrid;
      /// Array of ListLevel describing the several levels of the list.
      vector<ListLevel*>* rglvl;
	  /// A grfhic that specifies HTML incompatibilities of the list.
      unsigned char grfhic;
	  unsigned char* _rawBytes;

    public:
	  static const int LSTF_LENGTH = 28;
      static const short ISTD_NIL = 4095;
	  static const int VARIABLE_LENGTH = INT_MAX;

      virtual ~ListData();
	  /// Parses the StreamReader to retrieve a ListData
	  ListData( VirtualStreamReader* reader, int length );
  };
}