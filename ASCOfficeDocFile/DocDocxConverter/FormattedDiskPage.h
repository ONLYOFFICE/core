#pragma once

#include "../pole/pole.h"
//#include "../../Common/DocxFormat/Source/CompoundDocument/pole.h"
#include "../../DesktopEditor/common/Types.h"

namespace DocFileFormat
{
  typedef enum FKPType
  {
    Character,
    Paragraph
  } FKPType;

  class FormattedDiskPage 
  {
    friend class FormattedDiskPageCHPX;
	friend class FormattedDiskPagePAPX;
	friend class WordDocument;
  
    private:
      /// Specifies the type of the FKP
      FKPType Type;
      /// The WordDocument stream
      POLE::Stream* WordStream;
      /// Count of runs for that FKP
      unsigned char crun;
      /// Each value is the limit of a paragraph or run of exception text
      int* rgfc;
	  unsigned int rgfcSize;

    public:
	  FormattedDiskPage(): Type(Character), WordStream(NULL), crun(0), rgfc(NULL), rgfcSize(0)
	  {
	  }
  };
}
