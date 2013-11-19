#pragma once

#include "FormattedDiskPage.h"
#include "ParagraphHeight.h"
#include "ParagraphPropertyExceptions.h"
#include "FileInformationBlock.h"

namespace DocFileFormat
{
  typedef struct BX
  {
    byte wordOffset;
    ParagraphHeight phe;
  } BX;

  class FormattedDiskPagePAPX: public FormattedDiskPage
  {
    friend class MainDocumentMapping;
	friend class WordDocument;
	friend class HeaderMapping;
	friend class FooterMapping;
	friend class TextboxMapping;

    private:
      /// An array of the BX data structure.
      /// BX is a 13 byte data structure. The first byte of each is the word offset of the PAPX.
      BX* rgbx;
      /// grppapx consists of all of the PAPXs stored in FKP concatenated end to end. 
      /// Each PAPX begins with a count of words which records its length padded to a word boundary.
      unsigned int grppapxSize;
	  ParagraphPropertyExceptions** grppapx;

    public:
      virtual ~FormattedDiskPagePAPX();
	  FormattedDiskPagePAPX( IStream* wordStream, int offset, IStream* dataStream );
      /// Parses the 0Table (or 1Table) for FKP _entries containing PAPX
      static list<FormattedDiskPagePAPX*>* GetAllPAPXFKPs( FileInformationBlock* fib, IStream* wordStream, IStream* tableStream, IStream* dataStream);
      /// Returns a list of all PAPX FCs between they given boundaries.
      static list<int>* GetFileCharacterPositions( int fcMin, int fcMax, FileInformationBlock* fib, IStream* wordStream, IStream* tableStream, IStream* dataStream );
      /// Returnes a list of all ParagraphPropertyExceptions which correspond to text 
      /// between the given offsets.
      static list<ParagraphPropertyExceptions*>* GetParagraphPropertyExceptions( int fcMin, int fcMax, FileInformationBlock* fib, IStream* wordStream, IStream* tableStream, IStream* dataStream );
  };
}

//#endif //FORMATTED_DISK_PAGE_PAPX_H