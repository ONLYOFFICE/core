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

#include "Tplc.h"
#include "grfhic.h"

namespace ASCDocFileFormat
{
  class LSTF: public IOperand
  {
    private:
	  static const unsigned char SIZE_IN_BYTES = 28;
	  static const unsigned char RGISTD_PARA_COUNT = 9;
	  unsigned char bytes[SIZE_IN_BYTES];

    public:
	  LSTF()
      {
	    memset( this->bytes, 0, SIZE_IN_BYTES );
	  }

	  explicit LSTF( int _lsid, const Tplc* _tplc, bool _fSimpleList, 
		             bool _fAutoNum, bool _fHybrid, grfhic _grfhic, const vector<short>* _rgistdPara = NULL )
	  {
	    memset( this->bytes, 0, SIZE_IN_BYTES );

		FormatUtils::SetBytes( this->bytes, _lsid );

		if ( _tplc != NULL )
		{
		  FormatUtils::SetBytes( ( this->bytes + sizeof(_lsid) ), (unsigned int)(*_tplc) );
		}
		else
		{
		  FormatUtils::SetBytes( ( this->bytes + sizeof(_lsid) ), (unsigned int)0 );
		}

		if ( ( _rgistdPara != NULL ) && ( _rgistdPara->size() == RGISTD_PARA_COUNT ) )
		{
		  for ( unsigned int i = 0; i < RGISTD_PARA_COUNT; i++ )
		  {
		    FormatUtils::SetBytes( ( this->bytes + sizeof(_lsid) + sizeof(unsigned int) + ( i * sizeof(short) ) ), _rgistdPara->at( i ) );
		  }
		}
		else
		{
		  for ( unsigned int i = 0; i < RGISTD_PARA_COUNT; i++ )
		  {
		    FormatUtils::SetBytes( ( this->bytes + sizeof(_lsid) + sizeof(unsigned int) + ( i * sizeof(short) ) ), (short)0x0FFF );
		  }
		}

		unsigned char simpleListAutoNumHybridFlags = 0x00;
		unsigned char fSimpleList = 0x00;
		unsigned char fAutoNum = 0x00;
		unsigned char fHybrid = 0x00;

		( _fSimpleList ) ? ( fSimpleList = 0x01 ) : ( fSimpleList = 0x00 );
		( _fAutoNum ) ? ( fAutoNum = 0x04 ) : ( fAutoNum = 0x00 );
		( _fHybrid ) ? ( fHybrid = 0x10 ) : ( fHybrid = 0x00 );

        simpleListAutoNumHybridFlags |= ( fSimpleList | fAutoNum | fHybrid );

		this->bytes[sizeof(_lsid) + sizeof(unsigned int) + ( RGISTD_PARA_COUNT * sizeof(short) )] = simpleListAutoNumHybridFlags;
        this->bytes[sizeof(_lsid) + sizeof(unsigned int) + ( RGISTD_PARA_COUNT * sizeof(short) ) + sizeof(simpleListAutoNumHybridFlags)] = (unsigned char)_grfhic;
	  }

	  LSTF( const LSTF& _lSTF )
      {
	    memset( this->bytes, 0, SIZE_IN_BYTES );

		memcpy( this->bytes, _lSTF.bytes, SIZE_IN_BYTES );
	  }

	  virtual ~LSTF()
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
	    return sizeof(this->bytes);
	  }
  };
}