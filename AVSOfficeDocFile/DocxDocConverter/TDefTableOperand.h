#pragma once

#include "Constants.h"
#include "Brc80MayBeNil.h"
#include "XAS.h"

namespace AVSDocFileFormat
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

			this->bytes = new byte[this->sizeInBytes];

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
			this->bytes = new byte[this->sizeInBytes];

			memcpy_s( this->bytes, this->sizeInBytes, tc80.bytes, tc80.sizeInBytes );
		}

		virtual ~TC80()
		{
			RELEASEARRAYOBJECTS (bytes);
		}

		virtual operator byte*() const
		{
			return this->bytes;
		}

		virtual operator const byte*() const
		{
			return (const byte*)this->bytes;
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

		byte* bytes;
		unsigned int sizeInBytes;
	};

	class TDefTableOperand : public IOperand
	{

	public:
		static const byte MAX_NUMBER_OF_COLUMNS = 63;

		TDefTableOperand() : cb(sizeof(cb) + sizeof(NumberOfColumns) + 1), NumberOfColumns(0), bytes(NULL)
		{

		}

		TDefTableOperand( byte _numberOfColumns, const vector<XAS>& _rgdxaCenter, const vector<TC80>& _rgTc80 ) : rgdxaCenter(_rgdxaCenter), rgTc80(_rgTc80), bytes(NULL)
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

			this->bytes = new byte[this->cb + 1];

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
						( i * rgTc80[i].Size() ) ), (byte*)(rgTc80[i]), rgTc80[i].Size() );
				}
			}
		}

		TDefTableOperand( const TDefTableOperand& tdto ) : cb(tdto.cb), NumberOfColumns(tdto.NumberOfColumns), rgdxaCenter(tdto.rgdxaCenter), rgTc80(tdto.rgTc80)
		{
			this->bytes = new byte[this->cb + 1];

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

		virtual operator byte*() const
		{
			return this->bytes;
		}

		virtual operator const byte*() const
		{
			return (const byte*)this->bytes;
		}
	private:
		unsigned short cb;
		byte NumberOfColumns;
		vector<XAS> rgdxaCenter;
		vector<TC80> rgTc80;

		byte* bytes;
	};
}