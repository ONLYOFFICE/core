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

#include "Brc.h"
#include "Prl.h"

namespace ASCDocFileFormat
{
	class TableBordersOperand: public IOperand
	{
	private:
		unsigned char cb;
		Brc brcTop;
		Brc brcLeft;
		Brc brcBottom;
		Brc brcRight;
		Brc brcHorizontalInside;
		Brc brcVerticalInside;

		unsigned char* bytes;

		void Initialize()
		{
			RELEASEARRAYOBJECTS (bytes);

			this->cb = ( this->brcTop.Size() +  this->brcLeft.Size() + this->brcBottom.Size() + 
				this->brcRight.Size() + this->brcHorizontalInside.Size() + this->brcVerticalInside.Size() );

			this->bytes = new unsigned char[this->cb + 1];

			if ( this->bytes != NULL )
			{
				memset( this->bytes, 0, ( this->cb + 1 ) );

				this->bytes[0] = this->cb;

				memcpy( ( this->bytes + sizeof(this->cb) ), (unsigned char*)this->brcTop, this->brcTop.Size() );
				memcpy( ( this->bytes + sizeof(this->cb) + this->brcTop.Size() ), (unsigned char*)this->brcLeft, this->brcLeft.Size() );
				memcpy( ( this->bytes + sizeof(this->cb) + this->brcTop.Size() + this->brcLeft.Size() ), (unsigned char*)this->brcBottom, this->brcBottom.Size() );
				memcpy( ( this->bytes + sizeof(this->cb) + this->brcTop.Size() + this->brcLeft.Size() + this->brcBottom.Size() ), (unsigned char*)this->brcRight, this->brcRight.Size() );
				memcpy( ( this->bytes + sizeof(this->cb) + this->brcTop.Size() + this->brcLeft.Size() + this->brcBottom.Size() + this->brcRight.Size() ), (unsigned char*)this->brcHorizontalInside, this->brcHorizontalInside.Size() );
				memcpy( ( this->bytes + sizeof(this->cb) + this->brcTop.Size() + this->brcLeft.Size() + this->brcBottom.Size() + this->brcRight.Size() + this->brcHorizontalInside.Size() ), (unsigned char*)this->brcVerticalInside, this->brcVerticalInside.Size() );
			}
		}

	public:
		TableBordersOperand():
		  bytes(NULL), cb(0)
		  {
			  this->Initialize();
		  }

		  explicit TableBordersOperand( const Brc& _brcTop, const Brc& _brcLeft, const Brc& _brcBottom,
			  const Brc& _brcRight, const Brc& _brcHorizontalInside, const Brc& _brcVerticalInside ):
		  brcTop(_brcTop), brcLeft(_brcLeft), brcBottom(_brcBottom), brcRight(_brcRight),
			  brcHorizontalInside(_brcHorizontalInside), brcVerticalInside(_brcVerticalInside),
			  bytes(NULL), cb(0)
		  {
			  this->Initialize();  
		  }

		  TableBordersOperand( const TableBordersOperand& _tableBordersOperand ):
		  brcTop(_tableBordersOperand.brcTop), brcLeft(_tableBordersOperand.brcLeft), 
			  brcBottom(_tableBordersOperand.brcBottom), brcRight(_tableBordersOperand.brcRight),
			  brcHorizontalInside(_tableBordersOperand.brcHorizontalInside), 
			  brcVerticalInside(_tableBordersOperand.brcVerticalInside), bytes(NULL), cb(0)
		  {
			  this->bytes = new unsigned char[this->cb + 1];

			  memcpy_s( this->bytes, ( this->cb + 1 ), _tableBordersOperand.bytes, ( _tableBordersOperand.cb + 1 ) );
		  }

		  explicit TableBordersOperand( const Prl& _prl ):
		  bytes(NULL), cb(0)
		  {
			  unsigned long prlSize = 0;

			  unsigned char* bytes = _prl.GetBytes( &prlSize );

			  if ( bytes != NULL )
			  {
				  this->brcTop = Brc( bytes +  0 + sizeof(unsigned short) + sizeof(unsigned char) );
				  this->brcLeft = Brc( bytes + 8 + sizeof(unsigned short) + sizeof(unsigned char) );
				  this->brcBottom = Brc( bytes + 16 + sizeof(unsigned short) + sizeof(unsigned char) );
				  this->brcRight = Brc( bytes + 24 + sizeof(unsigned short) + sizeof(unsigned char) );
				  this->brcHorizontalInside = Brc( bytes + 32 + sizeof(unsigned short) + sizeof(unsigned char) );
				  this->brcVerticalInside = Brc( bytes + 40 + sizeof(unsigned short) + sizeof(unsigned char) );

				  RELEASEARRAYOBJECTS (bytes);
			  }

			  this->Initialize();
		  }

		  TableBordersOperand& operator = ( const TableBordersOperand& _tableBordersOperand )
		  {
			  if ( this != &_tableBordersOperand )
			  {
				  this->brcTop = _tableBordersOperand.brcTop;
				  this->brcLeft = _tableBordersOperand.brcLeft;
				  this->brcBottom = _tableBordersOperand.brcBottom;
				  this->brcRight = _tableBordersOperand.brcRight;
				  this->brcHorizontalInside = _tableBordersOperand.brcHorizontalInside;
				  this->brcVerticalInside = _tableBordersOperand.brcVerticalInside;

				  this->Initialize();
			  }

			  return *this;
		  }

		  void SetTopBorder( const Brc& _brcTop )
		  {
			  this->brcTop = _brcTop;

			  this->Initialize();
		  }

		  void SetLeftBorder( const Brc& _brcLeft )
		  {
			  this->brcLeft = _brcLeft;

			  this->Initialize();
		  }

		  void SetBottomBorder( const Brc& _brcBottom )
		  {
			  this->brcBottom = _brcBottom;

			  this->Initialize();
		  }

		  void SetRightBorder( const Brc& _brcRight )
		  {
			  this->brcRight = _brcRight;

			  this->Initialize();
		  }

		  void SetHorizontalInsideBorder( const Brc& _brcHorizontalInside )
		  {
			  this->brcHorizontalInside = _brcHorizontalInside;

			  this->Initialize();
		  }

		  void SetVerticalInsideBorder( const Brc& _brcVerticalInside )
		  {
			  this->brcVerticalInside = _brcVerticalInside;

			  this->Initialize();
		  }

		  virtual ~TableBordersOperand()
		  {
			  RELEASEARRAYOBJECTS (bytes);
		  }

		  virtual unsigned int Size() const
		  {
			  return ( this->cb + 1 );  
		  }

		  virtual operator unsigned char*() const
		  {
			  return this->bytes;
		  }

		  virtual operator const unsigned char*() const
		  {
			  return (const unsigned char*)this->bytes;
		  }
	};
}