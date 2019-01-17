/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
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
	  static const unsigned char SIZE_IN_BYTES = 28;
	  static const unsigned char RGBXCH_NUMS_COUNT = 9;
	  unsigned char bytes[SIZE_IN_BYTES];

    public:
      LVLF()
	  {
	    memset( this->bytes, 0, SIZE_IN_BYTES );
	  }

	  explicit LVLF( int _iStartAt, Constants::MSONFC _nfc, Constants::LevelJustification _jc, bool _fLegal,
		             bool _fNoRestart, bool _fIndentSav, bool _fConverted, bool _fTentative, 
					 Constants::CharacterFollows _ixchFollow, int _dxaIndentSav, unsigned char _ilvlRestartLim, grfhic _grfhic,
			         const vector<unsigned char>* _rgbxchNums = NULL /*LVL can set this value*/, unsigned char _cbGrpprlChpx = 0 /*LVL can set this value.*/, unsigned char _cbGrpprlPapx = 0 /*LVL can set this value.*/ )
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

		unsigned char nfc = 0x00;

		if ( ( _nfc != Constants::msonfcHex ) && ( _nfc != Constants::msonfcChiManSty ) && 
			 ( _nfc != Constants::msonfcSbChar ) && ( _nfc != Constants::msonfcDArabic ) )
		{
		  nfc = (unsigned char)_nfc;
		}

		this->bytes[offset] = nfc;
		offset += sizeof(nfc);

		unsigned char flags = (unsigned char)_jc;
		unsigned char fLegal = 0x00;
		unsigned char fNoRestart = 0x00;
		unsigned char fIndentSav = 0x00;
		unsigned char fConverted = 0x00;
		unsigned char fTentative = 0x00;

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
	      for ( vector<unsigned char>::const_iterator iter = _rgbxchNums->begin(); iter != _rgbxchNums->end(); iter++ )
		  {
		    this->bytes[offset] = *iter;
			offset += sizeof(*iter);
		  }
		}
		else
		{
		  offset += ( RGBXCH_NUMS_COUNT * sizeof(unsigned char) );
		}

		this->bytes[offset] = (unsigned char)_ixchFollow;
		offset += sizeof(unsigned char);

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

		this->bytes[offset] = (unsigned char)_grfhic;
		offset += sizeof(unsigned char);
      }

	  LVLF( const LVLF& _lVLF )
	  {
	    memset( this->bytes, 0, SIZE_IN_BYTES );

		memcpy( this->bytes, _lVLF.bytes, SIZE_IN_BYTES );
	  }

	  virtual ~LVLF()
	  {
	  }

	  virtual operator unsigned char*() const
	  {
	    return (unsigned char*)this->bytes;
	  }

	  virtual operator const unsigned char*() const
	  {
	    return (const unsigned char*)this->bytes;
	  }

	  virtual unsigned int Size() const
	  {
	    return sizeof(this->bytes);
	  }
  };
}