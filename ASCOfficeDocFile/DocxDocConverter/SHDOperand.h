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

#include "COLORREF.h"

namespace ASCDocFileFormat
{
  class Shd: public IOperand
  {
    private:
	  COLORREF cvFore;
	  COLORREF cvBack;
	  //!!!TODO!!!
	  unsigned short ipat;

	  static const unsigned char SIZE_IN_BYTES = 10;

	  unsigned char bytes[SIZE_IN_BYTES];

    public:
	  Shd():
      cvFore(), cvBack(), ipat(0)
	  {
	    memcpy( this->bytes, this->cvFore, this->cvFore.Size() );
		memcpy( ( this->bytes + this->cvFore.Size() ), (unsigned char*)this->cvBack, this->cvBack.Size() );
		memcpy( ( this->bytes + this->cvFore.Size() + this->cvBack.Size() ), (unsigned char*)(&this->ipat), sizeof(this->ipat) );
	  }

	  explicit Shd( const COLORREF& _cvFore, const COLORREF& _cvBack, unsigned short _ipat ):
      cvFore(_cvFore), cvBack(_cvBack), ipat(_ipat)
	  {
 	    memcpy( this->bytes, this->cvFore, this->cvFore.Size() );
		memcpy( ( this->bytes + this->cvFore.Size() ), (unsigned char*)this->cvBack, this->cvBack.Size() );
		memcpy( ( this->bytes + this->cvFore.Size() + this->cvBack.Size() ), (unsigned char*)(&this->ipat), sizeof(this->ipat) );
	  }

	  Shd( const Shd& _shd ):
	  cvFore(_shd.cvFore), cvBack(_shd.cvBack), ipat(_shd.ipat)
	  {
	    memcpy( this->bytes, this->cvFore, this->cvFore.Size() );
		memcpy( ( this->bytes + this->cvFore.Size() ), (unsigned char*)this->cvBack, this->cvBack.Size() );
		memcpy( ( this->bytes + this->cvFore.Size() + this->cvBack.Size() ), (unsigned char*)(&this->ipat), sizeof(this->ipat) );
	  }

	  virtual ~Shd()
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

  class SHDOperand: public IOperand
  {
    private:
	  static const unsigned char cb = 10;
	  Shd shd;

	  unsigned char bytes[SHDOperand::cb + 1];

    public:
	  SHDOperand():
	  shd()
	  {
	    this->bytes[0] = SHDOperand::cb;
		memcpy( ( this->bytes + sizeof(SHDOperand::cb) ), (unsigned char*)this->shd, this->shd.Size() );
	  }
 
	  explicit SHDOperand( const Shd& _shd ):
      shd(_shd)
	  {
	    this->bytes[0] = SHDOperand::cb;
		memcpy( ( this->bytes + sizeof(SHDOperand::cb) ), (unsigned char*)this->shd, this->shd.Size() );
	  }

	  SHDOperand( const SHDOperand& _shdOperand ):
	  shd(_shdOperand.shd)
	  {
	    this->bytes[0] = SHDOperand::cb;
		memcpy( ( this->bytes + sizeof(SHDOperand::cb) ), (unsigned char*)this->shd, this->shd.Size() );
	  }

	  virtual ~SHDOperand()
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