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

#include "Brc80MayBeNil.h"
#include "Prl.h"

namespace ASCDocFileFormat
{
	class TableBordersOperand80: public IOperand
	{
	public:
		TableBordersOperand80() : bytes(NULL), cb(0)
		{
			Initialize();
		}

		explicit TableBordersOperand80( Brc80MayBeNil _brcTop, Brc80MayBeNil _brcLeft, Brc80MayBeNil _brcBottom, Brc80MayBeNil _brcRight, Brc80MayBeNil _brcHorizontalInside, Brc80MayBeNil _brcVerticalInside ) : brcTop(_brcTop), brcLeft(_brcLeft), brcBottom(_brcBottom), brcRight(_brcRight), brcHorizontalInside(_brcHorizontalInside), brcVerticalInside(_brcVerticalInside), bytes(NULL), cb(0)
		{
			Initialize();  
		}

		TableBordersOperand80( const TableBordersOperand80& _tableBordersOperand80 ): brcTop(_tableBordersOperand80.brcTop), brcLeft(_tableBordersOperand80.brcLeft), brcBottom(_tableBordersOperand80.brcBottom), brcRight(_tableBordersOperand80.brcRight), brcHorizontalInside(_tableBordersOperand80.brcHorizontalInside), brcVerticalInside(_tableBordersOperand80.brcVerticalInside), bytes(NULL), cb(0)
		{
			this->bytes = new unsigned char[this->cb + 1];
			memcpy_s( this->bytes, ( this->cb + 1 ), _tableBordersOperand80.bytes, ( _tableBordersOperand80.cb + 1 ) );
		}

		explicit TableBordersOperand80( const Prl& _prl ):
		bytes(NULL), cb(0)
		{
			unsigned long prlSize = 0;

			unsigned char* bytes = _prl.GetBytes( &prlSize );

			if ( bytes != NULL )
			{
				brcTop					=	Brc80MayBeNil( FormatUtils::BytesToUInt32( bytes, ( 0 + sizeof(unsigned short) + sizeof(unsigned char) ), prlSize ) );
				brcLeft					=	Brc80MayBeNil( FormatUtils::BytesToUInt32( bytes, ( 4 + sizeof(unsigned short) + sizeof(unsigned char) ), prlSize ) );
				brcBottom				=	Brc80MayBeNil( FormatUtils::BytesToUInt32( bytes, ( 8 + sizeof(unsigned short) + sizeof(unsigned char) ), prlSize ) );
				brcRight				=	Brc80MayBeNil( FormatUtils::BytesToUInt32( bytes, ( 12 + sizeof(unsigned short) + sizeof(unsigned char) ), prlSize ) );
				brcHorizontalInside		=	Brc80MayBeNil( FormatUtils::BytesToUInt32( bytes, ( 16 + sizeof(unsigned short) + sizeof(unsigned char) ), prlSize ) );
				brcVerticalInside		=	Brc80MayBeNil( FormatUtils::BytesToUInt32( bytes, ( 20 + sizeof(unsigned short) + sizeof(unsigned char) ), prlSize ) );

				RELEASEARRAYOBJECTS (bytes);
			}

			this->Initialize();
		}

		TableBordersOperand80& operator = ( const TableBordersOperand80& _tableBordersOperand80 )
		{
			if ( this != &_tableBordersOperand80 )
			{
				this->brcTop = _tableBordersOperand80.brcTop;
				this->brcLeft = _tableBordersOperand80.brcLeft;
				this->brcBottom = _tableBordersOperand80.brcBottom;
				this->brcRight = _tableBordersOperand80.brcRight;
				this->brcHorizontalInside = _tableBordersOperand80.brcHorizontalInside;
				this->brcVerticalInside = _tableBordersOperand80.brcVerticalInside;

				this->Initialize();
			}

			return *this;
		}

		void SetTopBorder( const Brc80MayBeNil& _brc80MayBeNilTop )
		{
			this->brcTop = _brc80MayBeNilTop;

			this->Initialize();
		}

		void SetLeftBorder( const Brc80MayBeNil& _brc80MayBeNilLeft )
		{
			this->brcLeft = _brc80MayBeNilLeft;

			this->Initialize();
		}

		void SetBottomBorder( const Brc80MayBeNil& _brc80MayBeNilBottom )
		{
			this->brcBottom = _brc80MayBeNilBottom;

			this->Initialize();
		}

		void SetRightBorder( const Brc80MayBeNil& _brc80MayBeNilRight )
		{
			this->brcRight = _brc80MayBeNilRight;

			this->Initialize();
		}

		void SetHorizontalInsideBorder( const Brc80MayBeNil& _brc80MayBeNilHorizontalInside )
		{
			this->brcHorizontalInside = _brc80MayBeNilHorizontalInside;

			this->Initialize();
		}

		void SetVerticalInsideBorder( const Brc80MayBeNil& _brc80MayBeNilVerticalInside )
		{
			this->brcVerticalInside = _brc80MayBeNilVerticalInside;

			this->Initialize();
		}

		virtual ~TableBordersOperand80()
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

	private:
		void Initialize()
		{
			RELEASEARRAYOBJECTS (bytes);

			this->cb = ( 6 * sizeof(Brc80MayBeNil) );

			this->bytes = new unsigned char[this->cb + 1];

			if ( this->bytes != NULL )
			{
				memset( this->bytes, 0, ( this->cb + 1 ) );

				unsigned int brcTopValue = this->brcTop;
				unsigned int brcLeftValue = this->brcLeft;
				unsigned int brcBottomValue = this->brcBottom;
				unsigned int brcRightValue = this->brcRight;
				unsigned int brcHorizontalInsideValue = this->brcHorizontalInside;
				unsigned int brcVerticalInsideValue = this->brcVerticalInside;

				this->bytes[0] = this->cb;
				memcpy( ( this->bytes + sizeof(this->cb) ), &brcTopValue, sizeof(brcTopValue) );
				memcpy( ( this->bytes + sizeof(this->cb) + sizeof(brcTopValue) ), &brcLeftValue, sizeof(brcLeftValue) );
				memcpy( ( this->bytes + sizeof(this->cb) + sizeof(brcTopValue) + sizeof(brcLeftValue) ), &brcBottomValue, sizeof(brcBottomValue) );
				memcpy( ( this->bytes + sizeof(this->cb) + sizeof(brcTopValue) + sizeof(brcLeftValue) + sizeof(brcBottomValue) ), &brcRightValue, sizeof(brcRightValue) );
				memcpy( ( this->bytes + sizeof(this->cb) + sizeof(brcTopValue) + sizeof(brcLeftValue) + sizeof(brcBottomValue) + sizeof(brcRightValue) ), &brcHorizontalInsideValue, sizeof(brcHorizontalInsideValue) );
				memcpy( ( this->bytes + sizeof(this->cb) + sizeof(brcTopValue) + sizeof(brcLeftValue) + sizeof(brcBottomValue) + sizeof(brcRightValue) + sizeof(brcHorizontalInsideValue) ), &brcVerticalInsideValue, sizeof(brcVerticalInsideValue) );
			}
		}

	private:

		unsigned char cb;
		Brc80MayBeNil brcTop;
		Brc80MayBeNil brcLeft;
		Brc80MayBeNil brcBottom;
		Brc80MayBeNil brcRight;
		Brc80MayBeNil brcHorizontalInside;
		Brc80MayBeNil brcVerticalInside;

		unsigned char* bytes;
	};
}