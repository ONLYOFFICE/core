#pragma once

#include "Constants.h"
#include "grfhic.h"

namespace ASCDocFileFormat
{
  static const char NumberFormatCodes[60][29] =
  {
    "decimal",
    "upperRoman",
    "lowerRoman",
    "upperLetter",
    "lowerLetter",
    "ordinal",
    "cardinalText",
    "ordinalText",
    "hex",
    "chicago",
    "ideographDigital",
    "japaneseCounting",
    "aiueo",
    "iroha",
    "decimalFullWidth",
    "decimalHalfWidth",
    "japaneseLegal",
    "japaneseDigitalTenThousand",
    "decimalEnclosedCircle",
    "decimalFullWidth2",
    "aiueoFullWidth",
    "irohaFullWidth",
    "decimalZero",
    "bullet",
    "ganada",
    "chosung",
    "decimalEnclosedFullstop",
    "decimalEnclosedParen",
    "decimalEnclosedCircleChinese",
    "ideographEnclosedCircle",
    "ideographTraditional",
    "ideographZodiac",
    "ideographZodiacTraditional",
    "taiwaneseCounting",
    "ideographLegalTraditional",
    "taiwaneseCountingThousand",
    "taiwaneseDigital",
    "chineseCounting",
    "chineseLegalSimplified",
    "chineseCountingThousand",
    "koreanDigital",
    "koreanCounting",
    "koreanLegal",
    "koreanDigital2",
    "vietnameseCounting",
    "russianLower",
    "russianUpper",
    "none",
    "numberInDash",
    "hebrew1",
    "hebrew2",
    "arabicAlpha",
    "arabicAbjad",
    "hindiVowels",
    "hindiConsonants",
    "hindiNumbers",
    "hindiCounting",
    "thaiLetters",
    "thaiNumbers",
    "thaiCounting"
  };
	
  class LVLF: public IOperand
  {
    friend class LVL;
  
    private:
	  static const byte SIZE_IN_BYTES = 28;
	  static const byte RGBXCH_NUMS_COUNT = 9;
	  byte bytes[SIZE_IN_BYTES];

    public:
      LVLF()
	  {
	    memset( this->bytes, 0, SIZE_IN_BYTES );
	  }

	  explicit LVLF( int _iStartAt, Constants::MSONFC _nfc, Constants::LevelJustification _jc, bool _fLegal,
		             bool _fNoRestart, bool _fIndentSav, bool _fConverted, bool _fTentative, 
					 Constants::CharacterFollows _ixchFollow, int _dxaIndentSav, byte _ilvlRestartLim, grfhic _grfhic,
			         const vector<byte>* _rgbxchNums = NULL /*LVL can set this value*/, byte _cbGrpprlChpx = 0 /*LVL can set this value.*/, byte _cbGrpprlPapx = 0 /*LVL can set this value.*/ )
	  {
	    memset( this->bytes, 0, SIZE_IN_BYTES );

		unsigned int offset = 0;

		int iStartAt = 0;

		if ( _iStartAt > 0x7FFF )
		{
		  iStartAt = 0x7FFF;   
		}
		else if ( _iStartAt < 0 )
		{
		  iStartAt = 0;  
		}
		else
		{
		  iStartAt = _iStartAt;
		}

		FormatUtils::SetBytes( ( this->bytes + offset ), iStartAt );
		offset += sizeof(iStartAt);

		byte nfc = 0x00;

		if ( ( _nfc != Constants::msonfcHex ) && ( _nfc != Constants::msonfcChiManSty ) && 
			 ( _nfc != Constants::msonfcSbChar ) && ( _nfc != Constants::msonfcDArabic ) )
		{
		  nfc = (byte)_nfc;
		}

		this->bytes[offset] = nfc;
		offset += sizeof(nfc);

		byte flags = (byte)_jc;
		byte fLegal = 0x00;
		byte fNoRestart = 0x00;
		byte fIndentSav = 0x00;
		byte fConverted = 0x00;
		byte fTentative = 0x00;

		( _fLegal ) ? ( fLegal = 0x04 ) : ( fLegal = 0x00 );
		( _fNoRestart ) ? ( fNoRestart = 0x08 ) : ( fNoRestart = 0x00 );
		( _fIndentSav ) ? ( fIndentSav = 0x10 ) : ( fIndentSav = 0x00 );
		( _fConverted ) ? ( fConverted = 0x20 ) : ( fConverted = 0x00 );
		( _fTentative ) ? ( fTentative = 0x80 ) : ( fTentative = 0x00 );

		flags |= ( fLegal | fNoRestart | fIndentSav | fConverted | fTentative );
		this->bytes[offset] = flags;
		offset += sizeof(flags);

		if ( ( _rgbxchNums != NULL ) && ( _rgbxchNums->size() == RGBXCH_NUMS_COUNT ) )
		{
	      for ( vector<byte>::const_iterator iter = _rgbxchNums->begin(); iter != _rgbxchNums->end(); iter++ )
		  {
		    this->bytes[offset] = *iter;
			offset += sizeof(*iter);
		  }
		}
		else
		{
		  offset += ( RGBXCH_NUMS_COUNT * sizeof(byte) );
		}

		this->bytes[offset] = (byte)_ixchFollow;
		offset += sizeof(byte);

		int dxaIndentSav = 0;

		if ( _fIndentSav )
		{
		  if ( _dxaIndentSav > 0x00007BC0 )
		  {
		    dxaIndentSav = 0x00007BC0;   
		  }
		  else if ( _dxaIndentSav < 0xFFFF8440 )
		  {
		    dxaIndentSav = 0xFFFF8440;  
		  }
		  else
		  {
		    dxaIndentSav = _dxaIndentSav; 
		  }
		}

		FormatUtils::SetBytes( ( this->bytes + offset ), dxaIndentSav );
        offset += sizeof(dxaIndentSav);
        
		offset += sizeof(int); //MUST be ignored.

		this->bytes[offset] = _cbGrpprlChpx;
		offset += sizeof(_cbGrpprlChpx);

		this->bytes[offset] = _cbGrpprlPapx;
		offset += sizeof(_cbGrpprlPapx);

		this->bytes[offset] = _ilvlRestartLim;
		offset += sizeof(_ilvlRestartLim);

		this->bytes[offset] = (byte)_grfhic;
		offset += sizeof(byte);
      }

	  LVLF( const LVLF& _lVLF )
	  {
	    memset( this->bytes, 0, SIZE_IN_BYTES );

		memcpy( this->bytes, _lVLF.bytes, SIZE_IN_BYTES );
	  }

	  virtual ~LVLF()
	  {
	  }

	  virtual operator byte*() const
	  {
	    return (byte*)this->bytes;
	  }

	  virtual operator const byte*() const
	  {
	    return (const byte*)this->bytes;
	  }

	  virtual unsigned int Size() const
	  {
	    return sizeof(this->bytes);
	  }
  };
}