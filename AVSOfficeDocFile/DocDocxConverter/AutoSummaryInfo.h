#pragma once

namespace DocFileFormat
{
  class AutoSummaryInfo
  {
    private:
	  /// True if the ASUMYI is valid
      bool fValid;
      /// True if AutoSummary View is active
      bool fView;
      /// Display method for AutoSummary View:
      /// 0 = Emphasize in current doc
      /// 1 = Reduce doc to summary
      /// 2 = Insert into doc
      /// 3 = Show in new document
      short iViewBy;
      /// True if File Properties summary information 
      /// should be updated after the next summarization
      bool fUpdateProps;
      /// Dialog summary level
      short wDlgLevel;
      /// Upper bound for lLevel for sentences in this document
      int lHighestLevel;
	  /// Show document sentences at or below this level
      int lCurrentLevel;

    public:
	  virtual ~AutoSummaryInfo()
	  {
	  }

	  AutoSummaryInfo():
	  fValid(false), fView(false), iViewBy(0), fUpdateProps(false), wDlgLevel(0), 
	  lHighestLevel(0), lCurrentLevel(0)
	  {
	  }

      /// Parses the bytes to retrieve a AutoSummaryInfo
	  AutoSummaryInfo( byte* bytes, int size ):
	  fValid(false), fView(false), iViewBy(0), fUpdateProps(false), wDlgLevel(0), 
	  lHighestLevel(0), lCurrentLevel(0)
      {
        if ( size == 12 )
        {
          //split byte 0 and 1 into bits
          this->fValid = FormatUtils::GetBitFromBytes( bytes, size, 0 );
          this->fView = FormatUtils::GetBitFromBytes( bytes, size, 1 );
          this->iViewBy = (short)FormatUtils::GetUIntFromBytesBits( bytes, size, 2, 2 );
          this->fUpdateProps = FormatUtils::GetBitFromBytes( bytes, size, 4 );

          this->wDlgLevel = FormatUtils::BytesToInt16( bytes, 2, size );
          this->lHighestLevel = FormatUtils::BytesToInt32( bytes, 4, size );
          this->lCurrentLevel = FormatUtils::BytesToInt32( bytes, 8, size );
        }
        else
        {
          //throw new ByteParseException("Cannot parse the struct ASUMYI, the length of the struct doesn't match");
        }
      }
  };
}