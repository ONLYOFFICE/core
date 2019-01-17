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
#include "Brc80MayBeNil.h"
#include "XAS.h"

namespace ASCDocFileFormat
{
	union TCGRF
	{
	private:
		struct
		{
			unsigned short horzMerge:2;
			unsigned short textFlow:3;
			unsigned short vertMerge:2;
			unsigned short vertAlign:2;
			unsigned short ftsWidth:3;
			unsigned short fFitText:1;
			unsigned short fNoWrap:1;
			unsigned short fHideMark:1;
			unsigned short fUnused:1;
		} TCGRFStruct;
		unsigned short TCGRFShort;

	public:
		TCGRF() : TCGRFShort(0)
		{

		}

		explicit TCGRF( Constants::HorzMerge _horzMerge, Constants::TextFlow _textFlow, Constants::VerticalMergeFlag _verticalMergeFlag, Constants::VerticalAlign _verticalAlign, Constants::Fts _fts, bool _fFitText, bool _fNoWrap, bool _fHideMark )
		{
			this->TCGRFStruct.horzMerge = _horzMerge;
			this->TCGRFStruct.textFlow = _textFlow;
			this->TCGRFStruct.vertMerge = _verticalMergeFlag;
			this->TCGRFStruct.vertAlign = _verticalAlign;
			this->TCGRFStruct.ftsWidth = _fts;

			( _fFitText ) ? ( this->TCGRFStruct.fFitText = 1 ) : ( this->TCGRFStruct.fFitText = 0 );
			( _fNoWrap ) ? ( this->TCGRFStruct.fNoWrap = 1 ) : ( this->TCGRFStruct.fNoWrap = 0 );
			( _fHideMark ) ? ( this->TCGRFStruct.fHideMark = 1 ) : ( this->TCGRFStruct.fHideMark = 0 );

			this->TCGRFStruct.fUnused = 0;
		}

		unsigned short GetValue() const
		{
			return this->TCGRFShort;
		}
	};

	class TC80: public IOperand
	{
	public:
		TC80() : tcgrf(), wWidth(0), brcTop(), brcLeft(), brcBottom(), brcRight(), bytes(NULL), sizeInBytes(0)
		{

		}

		explicit TC80( const TCGRF& _tcgrf, unsigned short _wWidth, const Brc80MayBeNil& _brcTop, const Brc80MayBeNil& _brcLeft, const Brc80MayBeNil& _brcBottom, const Brc80MayBeNil& _brcRight ) : tcgrf(_tcgrf), wWidth(_wWidth), brcTop(_brcTop), brcLeft(_brcLeft), brcBottom(_brcBottom), brcRight(_brcRight), bytes(NULL)
		{
			this->sizeInBytes = ( sizeof(tcgrf) + sizeof(wWidth) + sizeof(brcTop) + sizeof(brcLeft) + sizeof(brcBottom) + sizeof(brcRight) );

			this->bytes = new unsigned char[this->sizeInBytes];

			memset( this->bytes, 0, this->sizeInBytes );

			unsigned short tcgrfValue = this->tcgrf.GetValue();
			unsigned int brcTopValue = this->brcTop;
			unsigned int brcLeftValue = this->brcLeft;
			unsigned int brcBottomValue = this->brcBottom;
			unsigned int brcRightValue = this->brcRight;

			memcpy( this->bytes, &tcgrfValue, sizeof(tcgrf) );
			memcpy( ( this->bytes + sizeof(tcgrf) ), &(this->wWidth), sizeof(this->wWidth) );
			memcpy( ( this->bytes + sizeof(tcgrf) + sizeof(this->wWidth) ), &brcTopValue, sizeof(brcTop) );
			memcpy( ( this->bytes + sizeof(tcgrf) + sizeof(this->wWidth) + sizeof(brcTop) ), &brcLeftValue, sizeof(brcLeft) );
			memcpy( ( this->bytes + sizeof(tcgrf) + sizeof(this->wWidth) + sizeof(brcTop) + sizeof(brcLeft) ), 
				&brcBottomValue, sizeof(brcBottom) );
			memcpy( ( this->bytes + sizeof(tcgrf) + sizeof(this->wWidth) + sizeof(brcTop) + sizeof(brcLeft) + sizeof(brcBottom) ), &brcRightValue, sizeof(brcRight) );
		}

		TC80( const TC80& tc80 ) : tcgrf(tc80.tcgrf), wWidth(tc80.wWidth), brcTop(tc80.brcTop), brcLeft(tc80.brcLeft), brcBottom(tc80.brcBottom), brcRight(tc80.brcRight), sizeInBytes(tc80.sizeInBytes)
		{
			this->bytes = new unsigned char[this->sizeInBytes];

			memcpy_s( this->bytes, this->sizeInBytes, tc80.bytes, tc80.sizeInBytes );
		}

		virtual ~TC80()
		{
			RELEASEARRAYOBJECTS (bytes);
		}

		virtual operator unsigned char*() const
		{
			return this->bytes;
		}

		virtual operator const unsigned char*() const
		{
			return (const unsigned char*)this->bytes;
		}

		virtual unsigned int Size() const
		{
			return this->sizeInBytes;  
		}
	private:

		TCGRF tcgrf;
		unsigned short wWidth;
		Brc80MayBeNil brcTop;
		Brc80MayBeNil brcLeft;
		Brc80MayBeNil brcBottom;
		Brc80MayBeNil brcRight;

		unsigned char* bytes;
		unsigned int sizeInBytes;
	};

	class TDefTableOperand : public IOperand
	{

	public:
		static const unsigned char MAX_NUMBER_OF_COLUMNS = 63;

		TDefTableOperand() : cb(sizeof(cb) + sizeof(NumberOfColumns) + 1), NumberOfColumns(0), bytes(NULL)
		{

		}

		TDefTableOperand( unsigned char _numberOfColumns, const vector<XAS>& _rgdxaCenter, const vector<TC80>& _rgTc80 ) : rgdxaCenter(_rgdxaCenter), rgTc80(_rgTc80), bytes(NULL)
		{
			if ( _numberOfColumns > MAX_NUMBER_OF_COLUMNS )
			{
				this->NumberOfColumns = MAX_NUMBER_OF_COLUMNS;
			}
			else
			{
				this->NumberOfColumns = _numberOfColumns;
			}

			this->cb = ( sizeof(this->NumberOfColumns) + ( sizeof(XAS) * this->rgdxaCenter.size() ) + ( this->rgTc80.front().Size() * this->rgTc80.size() ) + 1 );

			this->bytes = new unsigned char[this->cb + 1];

			if ( this->bytes != NULL )
			{
				memset( this->bytes, 0, ( this->cb + 1 ) );

				memcpy( this->bytes, &this->cb, sizeof(this->cb) );
				memcpy( ( this->bytes + sizeof(this->cb) ), &this->NumberOfColumns, sizeof(this->NumberOfColumns) );

				for ( unsigned int i = 0; i < this->rgdxaCenter.size(); i++ )
				{
					short xasValue = rgdxaCenter[i];
					memcpy( ( this->bytes + sizeof(this->cb) + sizeof(this->NumberOfColumns) + ( i * sizeof(xasValue) ) ), &xasValue, sizeof(xasValue) );
				}

				for ( unsigned int i = 0; i < this->rgTc80.size(); i++ )
				{
					memcpy( ( this->bytes + sizeof(this->cb) + sizeof(this->NumberOfColumns) + ( this->rgdxaCenter.size() * sizeof(XAS) ) +
						( i * rgTc80[i].Size() ) ), (unsigned char*)(rgTc80[i]), rgTc80[i].Size() );
				}
			}
		}

		TDefTableOperand( const TDefTableOperand& tdto ) : cb(tdto.cb), NumberOfColumns(tdto.NumberOfColumns), rgdxaCenter(tdto.rgdxaCenter), rgTc80(tdto.rgTc80)
		{
			this->bytes = new unsigned char[this->cb + 1];

			if ( this->bytes != NULL )
			{
				memset( this->bytes, 0, ( this->cb + 1 ) );

				memcpy_s( this->bytes, ( this->cb + 1 ), tdto.bytes, ( tdto.cb + 1 ) );
			}
		}

		virtual ~TDefTableOperand()
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
		unsigned short cb;
		unsigned char NumberOfColumns;
		vector<XAS> rgdxaCenter;
		vector<TC80> rgTc80;

		unsigned char* bytes;
	};
}