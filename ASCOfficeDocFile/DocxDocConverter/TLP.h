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

namespace ASCDocFileFormat
{
  union Fatl
  {
    private:
	  struct
      {
        short fatlBorders:1;
	    short fatlShading:1;
	    short fatlFont:1;
	    short fatlColor:1;
	    short fatlBestFit:1;
	    short fatlHdrRows:1;
	    short fatlLastRow:1;
	    short fatlHdrCols:1;
	    short fatlLastCol:1;
	    short fatlNoRowBands:1;
	    short fatlNoColBands:1;
	    short padding:5;
      } FatlStruct;
	  short FatlShort;

    public:
	  Fatl():
	  FatlShort(0)
	  {
	  }

	  explicit Fatl( bool _fatlBorders, bool _fatlShading, bool _fatlFont, bool _fatlColor,
		             bool _fatlBestFit, bool _fatlHdrRows, bool _fatlLastRow, bool _fatlHdrCols, 
					 bool _fatlLastCol, bool _fatlNoRowBands, bool _fatlNoColBands ): 
	  FatlShort(0)
	  {
	    ( _fatlBorders ) ? ( this->FatlStruct.fatlBorders = 1 ) : ( this->FatlStruct.fatlBorders = 0 );
	    ( _fatlShading ) ? ( this->FatlStruct.fatlShading = 1 ) : ( this->FatlStruct.fatlShading = 0 );
	    ( _fatlFont ) ? ( this->FatlStruct.fatlFont = 1 ) : ( this->FatlStruct.fatlFont = 0 );
	    ( _fatlColor ) ? ( this->FatlStruct.fatlColor = 1 ) : ( this->FatlStruct.fatlColor = 0 );
	    ( _fatlBestFit ) ? ( this->FatlStruct.fatlBestFit = 1 ) : ( this->FatlStruct.fatlBestFit = 0 );
	    ( _fatlHdrRows ) ? ( this->FatlStruct.fatlHdrRows = 1 ) : ( this->FatlStruct.fatlHdrRows = 0 );
	    ( _fatlLastRow ) ? ( this->FatlStruct.fatlLastRow = 1 ) : ( this->FatlStruct.fatlLastRow = 0 );
	    ( _fatlHdrCols ) ? ( this->FatlStruct.fatlHdrCols = 1 ) : ( this->FatlStruct.fatlHdrCols = 0 );
	    ( _fatlLastCol ) ? ( this->FatlStruct.fatlLastCol = 1 ) : ( this->FatlStruct.fatlLastCol = 0 );
	    ( _fatlNoRowBands ) ? ( this->FatlStruct.fatlNoRowBands = 1 ) : ( this->FatlStruct.fatlNoRowBands = 0 );
	    ( _fatlNoColBands ) ? ( this->FatlStruct.fatlNoColBands = 1 ) : ( this->FatlStruct.fatlNoColBands = 0 );
	  }
 
	  explicit Fatl( short _fatl ):
	  FatlShort(_fatl)
	  {
	  }

	  operator short() const
	  {
	    return this->FatlShort;
	  }
  };
	
  class TLP: public IOperand
  {
    private:
	  Constants::Itl itl;
	  Fatl grfatl;

	  static const unsigned char SIZE_IN_BYTES = 4;

	  unsigned char bytes[SIZE_IN_BYTES];

    public:
	  TLP(): 
	  itl(Constants::itlNil), grfatl()
	  {
	    memset( this->bytes, 0, SIZE_IN_BYTES );

		memcpy( this->bytes, &(this->itl), 2 );
		short fatlValue = this->grfatl;
		memcpy( ( this->bytes + 2 ), &fatlValue, sizeof(fatlValue) );
	  }

	  explicit TLP( Constants::Itl _itl, Fatl _grfatl ):
	  itl(_itl), grfatl(_grfatl)
	  {
	    memset( this->bytes, 0, SIZE_IN_BYTES );

		memcpy( this->bytes, &(this->itl), 2 );
		short fatlValue = this->grfatl;
		memcpy( ( this->bytes + 2 ), &fatlValue, sizeof(fatlValue) );
	  }

	  virtual ~TLP()
	  {
	  }

	  virtual operator unsigned char*() const
	  {
	    return (unsigned char*)(this->bytes);
	  }

	  virtual operator const unsigned char*() const
	  {
	    return (const unsigned char*)this->bytes;
	  }

	  virtual unsigned int Size() const
	  {
	    return SIZE_IN_BYTES;
	  }
  };
}