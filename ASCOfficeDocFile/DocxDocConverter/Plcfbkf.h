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

#include "PLC.h"

namespace ASCDocFileFormat
{
  union BKC
  {
    private:
	  struct
	  {
	    unsigned short itcFirst:7;
		unsigned short fPub:1;
		unsigned short itcLim:6;
		unsigned short fNative:1;
		unsigned short fCol:1;
	  } BKCStruct;
	  unsigned short BKCUnsignedShort;

    public:
	  explicit BKC( unsigned short _bkc = 0 ):
	  BKCUnsignedShort(_bkc)
	  {
	  }

	  explicit BKC( unsigned char _itcFirst, unsigned char _itcLim, bool _fNative, bool _fCol ):
      BKCUnsignedShort(0)
	  {
	    this->BKCStruct.itcFirst = _itcFirst;
		this->BKCStruct.itcLim = _itcLim;

		( _fNative ) ? ( this->BKCStruct.fNative = 1 ) : ( this->BKCStruct.fNative = 0 );
		( _fCol ) ? ( this->BKCStruct.fCol = 1 ) : ( this->BKCStruct.fCol = 0 );
	  }

	  operator unsigned short () const
	  {
	    return this->BKCUnsignedShort;
	  }
  };
	
  class FBKF
  {
    private:
	  static const unsigned char SIZE_IN_BYTES = 4;
	  unsigned char bytes[SIZE_IN_BYTES];

    public:
	  FBKF()
	  {
	    memset( this->bytes, 0, SIZE_IN_BYTES );
	  }

	  explicit FBKF( unsigned short _ibkl, BKC _bkc )
	  {
	    FormatUtils::SetBytes( this->bytes, _ibkl );
		FormatUtils::SetBytes( ( this->bytes + sizeof(_ibkl) ), (unsigned short)_bkc );
	  }

      FBKF( const FBKF& _fBKF )
	  {
	    memset( this->bytes, 0, SIZE_IN_BYTES );

		memcpy( this->bytes, _fBKF.bytes, SIZE_IN_BYTES );
	  }

      FBKF& operator = ( const FBKF& _fBKF )
	  {
	    if ( this != &_fBKF )
		{
		  memset( this->bytes, 0, SIZE_IN_BYTES );

		  memcpy( this->bytes, _fBKF.bytes, SIZE_IN_BYTES );
		}

		return *this;
	  }

      unsigned short GetIndex() const
	  {
	    return FormatUtils::BytesToUInt16( (unsigned char*)(this->bytes), 0, SIZE_IN_BYTES );
	  }

	  BKC GetBKC() const
	  {
	    return BKC( FormatUtils::BytesToUInt16( (unsigned char*)(this->bytes), 2, SIZE_IN_BYTES ) );
	  }
  };

  typedef PLC<FBKF> Plcfbkf;
  typedef PLC<> Plcfbkl;
}