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

#include "MFPF.h"
#include "PICF_Shape.h"
#include "PICMID.h"

namespace ASCDocFileFormat
{
  class PICF: public IOperand
  {
    private:
      MFPF mfpf;

      static const unsigned char SIZE_IN_BYTES = 68;
	  unsigned char bytes[SIZE_IN_BYTES];

    public:
	  PICF():
	  mfpf(Constants::MM_SHAPE)
	  {
	    memset( this->bytes, 0, SIZE_IN_BYTES );

		unsigned int offset = ( sizeof(int) + sizeof(unsigned short) );

		memcpy( ( this->bytes + offset ), (unsigned char*)(this->mfpf), this->mfpf.Size() );
		offset += this->mfpf.Size();
	  }

	  explicit PICF( int dataSize /*( lcb - cbHeader )*/, const PICMID& _picmid, const MFPF& _mfpf = MFPF(), const PICF_Shape& _innerHeader = PICF_Shape() ):
	  mfpf(_mfpf)
	  {
	    memset( this->bytes, 0, SIZE_IN_BYTES );

	    unsigned int offset = 0;

	    FormatUtils::SetBytes( ( this->bytes + offset ), (int)( SIZE_IN_BYTES + dataSize ) );
	    offset += sizeof(dataSize);

	    FormatUtils::SetBytes( ( this->bytes + offset ), (unsigned short)(SIZE_IN_BYTES) );
	    offset += sizeof(unsigned short);

		memcpy( ( this->bytes + offset ), (unsigned char*)(this->mfpf), this->mfpf.Size() );
		offset += this->mfpf.Size();

   	    memcpy( ( this->bytes + offset ), (unsigned char*)_innerHeader, _innerHeader.Size() );
	    offset += _innerHeader.Size();

        memcpy( ( this->bytes + offset ), (unsigned char*)_picmid, _picmid.Size() );
	    offset += _picmid.Size();
	  }

	  PICF( const PICF& _pICF )
	  {
	    memset( this->bytes, 0, SIZE_IN_BYTES );

	    memcpy( this->bytes, _pICF.bytes, SIZE_IN_BYTES );
	  }

	  virtual ~PICF()
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

	  Constants::PictureDataFormat GetPictureDataFormat() const
	  {
	    return (Constants::PictureDataFormat)(this->mfpf);  
	  }
  };
}