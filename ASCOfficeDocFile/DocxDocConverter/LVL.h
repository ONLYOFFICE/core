#pragma once

#include "LVLF.h"
#include "Prl.h"
#include "Xstz.h"

namespace ASCDocFileFormat
{
	class LVL : public IOperand
	{
	public:

		LVL() : bytes(NULL), sizeInBytes(0)
		{

		}

		explicit LVL( const LVLF& _lvlf, const vector<Prl>& _grpprlPapx, const vector<Prl>& _grpprlChpx, const Xst& _xst ) : bytes(NULL), sizeInBytes(0)
		{
			LVLF lvlf( _lvlf );
			unsigned int lvlfSize = lvlf.Size();

			this->sizeInBytes = lvlfSize;

			unsigned int cbGrpprlChpx = 0;
			unsigned int cbGrpprlPapx = 0;

			for ( vector<Prl>::const_iterator iter = _grpprlPapx.begin(); iter != _grpprlPapx.end(); iter++ )
			{
				cbGrpprlPapx += iter->Size();  
			}

			for ( vector<Prl>::const_iterator iter = _grpprlChpx.begin(); iter != _grpprlChpx.end(); iter++ )
			{
				cbGrpprlChpx += iter->Size();  
			}

			this->sizeInBytes += cbGrpprlChpx;
			this->sizeInBytes += cbGrpprlPapx;

			//Get one-based character offset to a level placeholder in xst.rgtchar.
			const vector<byte> rgbxchNums = this->GetRgbxchNums( _xst );
			//rgbxchNums starts at 6 byte. Size MUST be 9 bytes.
			unsigned int rgbxchNumsOffset = 6;

			//Write real rgbxchNums to the lvlf.
			for ( vector<byte>::const_iterator iter = rgbxchNums.begin(); iter != rgbxchNums.end(); iter++ )
			{
				lvlf.bytes[rgbxchNumsOffset++] = *iter;
			}

			//Write real size of grpprlPapx ( 25 byte ) and grpprlChpx ( 24 byte ) to the lvlf.
			lvlf.bytes[24] = cbGrpprlChpx;
			lvlf.bytes[25] = cbGrpprlPapx;

			this->sizeInBytes += _xst.Size();

			if ( this->sizeInBytes != 0 )
			{
				this->bytes = new byte[this->sizeInBytes];

				if ( this->bytes != NULL )
				{
					memset( this->bytes, 0, this->sizeInBytes );

					memcpy( this->bytes, (byte*)lvlf, lvlfSize );

					byte* prlBytes = NULL;
					unsigned long prlSize = 0;
					unsigned int prlPrevSize = 0; 

					for ( unsigned int i = 0; i < _grpprlPapx.size(); i++ )
					{
						prlBytes = _grpprlPapx[i].GetBytes( &prlSize );

						if ( prlBytes != NULL )
						{
							memcpy( ( this->bytes + lvlfSize + prlPrevSize ), prlBytes, prlSize );
							prlPrevSize += prlSize;

							RELEASEARRAYOBJECTS (prlBytes);
						}
					}

					prlBytes = NULL;
					prlSize = 0;
					prlPrevSize = 0;

					for ( unsigned int i = 0; i < _grpprlChpx.size(); i++ )
					{
						prlBytes = _grpprlChpx[i].GetBytes( &prlSize );

						if ( prlBytes != NULL )
						{
							memcpy( ( this->bytes + lvlfSize + cbGrpprlPapx + prlPrevSize ), prlBytes, prlSize );
							prlPrevSize += prlSize;
		
							RELEASEARRAYOBJECTS (prlBytes);
						}
					}

					memcpy( ( this->bytes + lvlfSize + cbGrpprlPapx + cbGrpprlChpx ), (byte*)_xst, _xst.Size() );
				}
			}
		}

		LVL( const LVL& _lVL ) : bytes(NULL), sizeInBytes(_lVL.sizeInBytes)
		{
			this->bytes = new byte[this->sizeInBytes];

			if ( this->bytes != NULL )
			{
				memset( this->bytes, 0, this->sizeInBytes );

				memcpy( this->bytes, _lVL.bytes, this->sizeInBytes );
			}
		}

		LVL& operator = ( const LVL& _lVL )
		{
			if ( this != &_lVL )
			{
				RELEASEARRAYOBJECTS (bytes);

				this->sizeInBytes = _lVL.sizeInBytes;

				if ( this->sizeInBytes != 0 )
				{
					this->bytes = new byte[this->sizeInBytes];

					if ( this->bytes != NULL )
					{
						memset( this->bytes, 0, this->sizeInBytes );
						memcpy( this->bytes, _lVL.bytes, this->sizeInBytes );
					}
				}
			}

			return *this;
		}

		virtual ~LVL()
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
		const vector<byte> GetRgbxchNums( const Xst& _xst ) const
		{
			vector<byte> rgbxchNums( 9, 0 );

			vector<wchar_t> xstElements = _xst.GetElements();

			for ( unsigned int i = 0, j = 0; i < xstElements.size(); i++ )
			{
				if ( ( xstElements[i] == (wchar_t)0x0000 ) || ( xstElements[i] == (wchar_t)0x0001 ) || ( xstElements[i] == (wchar_t)0x0002 ) || 
					( xstElements[i] == (wchar_t)0x0003 ) || ( xstElements[i] == (wchar_t)0x0004 ) || ( xstElements[i] == (wchar_t)0x0005 ) || 
					( xstElements[i] == (wchar_t)0x0006 ) || ( xstElements[i] == (wchar_t)0x0007 ) || ( xstElements[i] == (wchar_t)0x0008 ) )
				{
					rgbxchNums[j++] = ( i + 1 );
				}
			}

			return rgbxchNums;
		}

	private:

		byte* bytes;
		unsigned int sizeInBytes;
	};
}