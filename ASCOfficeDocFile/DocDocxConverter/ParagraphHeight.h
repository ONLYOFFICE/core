#pragma once

namespace DocFileFormat
{
  class ParagraphHeight
  {
    private:
      /// Complex shape layout in this paragraph
      bool fVolatile;
      /// ParagraphHeight is valid when fUnk is true
      bool fUnk;
      /// When true, total height of paragraph is known but lines in 
      /// paragraph have different heights
      bool fDiffLines;
      /// When fDiffLines is 0, is number of lines in paragraph
      short clMac;
      /// Width of lines in paragraph
      int dxaCol;
      /// When fDiffLines is true, is height of every line in paragraph in pixels
      int dymLine;
      /// When fDiffLines is true, is the total height in pixels of the paragraph
      int dymHeight;
      /// If not == 0, used as a hint when finding the next row.
      /// (this value is only set if the PHE is stored in a PAP whose fTtp field is set)
      short dcpTtpNext;
      /// Height of table row.
      /// (this value is only set if the PHE is stored in a PAP whose fTtp field is set)
      int dymTableHeight;
      /// Reserved
      bool fSpare;
      /// Creates a new empty ParagraphHeight with default values

    public:
	  ParagraphHeight();
	  /// Parses the bytes to retrieve a ParagraphHeight
      /// The flag which indicates if the 
      /// ParagraphHeight is stored in a ParagraphProperties whose fTtp field is set
      ParagraphHeight( byte* bytes, int size, bool fTtpMode );

    private:
      void setDefaultValues();
  };
}
