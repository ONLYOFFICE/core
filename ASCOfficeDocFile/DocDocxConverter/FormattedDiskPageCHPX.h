#pragma once

#include "FormattedDiskPage.h"
#include "CharacterPropertyExceptions.h"
#include "FileInformationBlock.h"

namespace DocFileFormat
{
  class FormattedDiskPageCHPX: public FormattedDiskPage
  {
    friend class WordDocument;
  
    private:
	  /// An array of bytes where each byte is the word offset of a CHPX.
      byte* rgb;
      unsigned int grpchpxSize;
	  /// Consists all of the CHPXs stored in this FKP.
      CharacterPropertyExceptions** grpchpx;

    public:
	  virtual ~FormattedDiskPageCHPX();
      FormattedDiskPageCHPX( IStream* wordStream, int offset );
      /// Parses the 0Table (or 1Table) for FKP _entries containing CHPX
      static list<FormattedDiskPageCHPX*>* GetAllCHPXFKPs( FileInformationBlock* fib, IStream* wordStream, IStream* tableStream );
  };
}