#pragma once

#include "ItcFirstLim.h"
#include "Constants.h"

namespace ASCDocFileFormat
{
	class CSSA: public IOperand
	{
	private:
		ItcFirstLim itc;
		Constants::Fbrc grfbrc;
		Constants::Fts ftsWidth;
		unsigned short wWidth;

		static const byte SIZE_IN_BYTES = 6;
		byte bytes[SIZE_IN_BYTES];

	public:
		explicit CSSA( ItcFirstLim _itc, Constants::Fbrc _grfbrc, Constants::Fts _ftsWidth, unsigned short _wWidth ): itc(_itc), grfbrc(_grfbrc), ftsWidth(_ftsWidth)
		{
			if ( this->ftsWidth == Constants::ftsNil )
			{
				this->wWidth = 0;
			}
			else
			{
				this->wWidth = _wWidth;
			}

			memset( this->bytes, 0, sizeof(this->bytes) );
			memcpy( this->bytes, (void*)(&this->itc), sizeof(this->itc) );
			memcpy( ( this->bytes + sizeof(this->itc) ), &(this->grfbrc), sizeof(byte) );
			memcpy( ( this->bytes + sizeof(this->itc) + sizeof(byte) ), &(this->ftsWidth), sizeof(byte) );
			memcpy( ( this->bytes + sizeof(this->itc) + sizeof(byte) + sizeof(byte) ), &(this->wWidth), sizeof(this->wWidth) );
		}

		CSSA( const CSSA& _cssa ):
		itc(_cssa.itc), grfbrc(_cssa.grfbrc), ftsWidth(_cssa.ftsWidth), wWidth(_cssa.wWidth)
		{
			memset( this->bytes, 0, sizeof(this->bytes) );
			memcpy( this->bytes, (void*)(&this->itc), sizeof(this->itc) );
			memcpy( ( this->bytes + sizeof(this->itc) ), &(this->grfbrc), sizeof(byte) );
			memcpy( ( this->bytes + sizeof(this->itc) + sizeof(byte) ), &(this->ftsWidth), sizeof(byte) );
			memcpy( ( this->bytes + sizeof(this->itc) + sizeof(byte) + sizeof(byte) ), &(this->wWidth), sizeof(this->wWidth) );
		}

		virtual ~CSSA()
		{
		}

		virtual operator byte*() const
		{
			return (byte*)this->bytes;
		}

		virtual operator const byte*() const
		{
			return (const byte*)this->bytes;
		}

		virtual unsigned int Size() const
		{
			return sizeof(this->bytes);
		}
	};

	class CSSAOperand: public IOperand
	{
	private:
		CSSA cssa;

		byte *bytes;

	public:
		explicit CSSAOperand( const CSSA& _cssa ): cssa(_cssa), bytes(NULL)
		{
			byte cssaSize = this->cssa.Size();
			this->bytes = new byte[cssaSize + 1];
			memset( this->bytes, 0, ( cssaSize + 1 ) );
			this->bytes[0] = cssaSize;
			memcpy( ( this->bytes + sizeof(byte) ), this->cssa, cssaSize );
		}

		CSSAOperand( const CSSAOperand& _CSSAOperand ): cssa(_CSSAOperand.cssa), bytes(NULL)
		{
			byte cssaSize = this->cssa.Size();
			this->bytes = new byte[cssaSize + 1];
			memset( this->bytes, 0, ( cssaSize + 1 ) );
			this->bytes[0] = cssaSize;
			memcpy( ( this->bytes + sizeof(byte) ), this->cssa, cssaSize );
		}

		virtual ~CSSAOperand()
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
			return ( this->cssa.Size() + 1 );
		}
	};
}