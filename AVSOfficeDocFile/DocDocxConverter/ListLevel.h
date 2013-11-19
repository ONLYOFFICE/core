#pragma once

#include "ParagraphPropertyExceptions.h"
#include "CharacterPropertyExceptions.h"
#include "VirtualStreamReader.h"

namespace DocFileFormat
{
  typedef enum _FollowingChar
  {
    tab = 0,
    space,
    nothing
  } FollowingChar;

  static const WCHAR FollowingCharMap[3][8] =
  {
    _T( "tab" ),
    _T( "space" ),
    _T( "nothing" )
  };

  class ListLevel
  {
    friend class NumberingMapping;

    private:  
      /// Start at value for this list level
      int iStartAt;
      /// Number format code (see anld.nfc for a list of options)
      byte nfc;
      /// Alignment (left, right, or centered) of the paragraph number.
      byte jc;
      /// True if the level turns all inherited numbers to arabic, 
      /// false if it preserves their number format code (nfc)
      bool fLegal;
      /// True if the level‘s number sequence is not restarted by 
      /// higher (more significant) levels in the list
      bool fNoRestart;
      /// Word 6.0 compatibility option: equivalent to anld.fPrev (see ANLD)
      bool fPrev;
      /// Word 6.0 compatibility option: equivalent to anld.fPrevSpace (see ANLD)
      bool fPrevSpace;
      /// True if this level was from a converted Word 6.0 document.
      /// If it is true, all of the Word 6.0 compatibility options become 
      /// valid otherwise they are ignored.
      bool fWord6;
      /// Contains the character offsets into the LVL’s XST of the inherited numbers of previous levels.
      /// The XST contains place holders for any paragraph numbers contained in the text of the number, 
      /// and the place holder contains the ilvl of the inherited number, 
      /// so lvl.xst[lvl.rgbxchNums[0]] == the level of the first inherited number in this level.
      vector<byte> rgbxchNums;
      /// The type of character following the number text for the paragraph.
      FollowingChar ixchFollow;
      /// Word 6.0 compatibility option: equivalent to anld.dxaSpace (see ANLD).
      /// For newer versions indent to remove if we remove this numbering.
      int dxaSpace;
      /// Word 6.0 compatibility option: equivalent to anld.dxaIndent (see ANLD).
      /// Unused in newer versions.
      int dxaIndent;
      /// Length, in bytes, of the LVL‘s grpprlChpx.
      byte cbGrpprlChpx;
      /// Length, in bytes, of the LVL‘s grpprlPapx.
      byte cbGrpprlPapx;
      /// Limit of levels that we restart after.
      byte ilvlRestartLim;
      /// A grfhic that specifies HTML incompatibilities of the level.
      byte grfhic;
	  ParagraphPropertyExceptions* grpprlPapx;
      CharacterPropertyExceptions* grpprlChpx;
      wstring xst;

	  byte* _rawBytes;

    public:
      virtual ~ListLevel();
      /// Parses the given StreamReader to retrieve a LVL struct
	  ListLevel( VirtualStreamReader* reader, int length );
  };
}