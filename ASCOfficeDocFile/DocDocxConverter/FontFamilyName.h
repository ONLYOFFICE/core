#pragma once

#include "VirtualStreamReader.h"
#include "ByteStructure.h"

namespace DocFileFormat
{
  typedef struct FontSignature
  {
    unsigned int UnicodeSubsetBitfield0;
    unsigned int UnicodeSubsetBitfield1;
    unsigned int UnicodeSubsetBitfield2;
    unsigned int UnicodeSubsetBitfield3;
    unsigned int CodePageBitfield0;
    unsigned int CodePageBitfield1;
  } FontSignature;

  class FontFamilyName: public ByteStructure
  {
    friend class CharacterPropertiesMapping;
	friend class DocumentMapping;
	friend class FontTableMapping;
	friend class StyleSheetMapping;
  
    private:
      /// When true, font is a TrueType font
      bool fTrueType;
      /// Font family id
      byte ff;
      /// Base weight of font
      short wWeight;
      /// Character set identifier
      byte chs;
      /// Pitch request
      byte prq;
      /// Name of font
      wstring xszFtn;
      /// Alternative name of the font
      wstring xszAlt;
      /// Panose
      byte *panose;
	  /// Panose size
	  unsigned int panoseSize;
      /// Font sinature
      FontSignature fs;
	  
    public:
	  FontFamilyName();
	  virtual ~FontFamilyName();
	  virtual ByteStructure* ConstructObject( VirtualStreamReader* reader, int length );
	
    private:
	  long searchTerminationZero( VirtualStreamReader *reader );
  };
}
