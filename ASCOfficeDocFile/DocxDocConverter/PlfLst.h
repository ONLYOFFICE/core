#pragma once

#include "LSTF.h"

namespace ASCDocFileFormat
{
	class PlfLst : public IOperand
	{
	public:
		PlfLst() : bytes(NULL), sizeInBytes(2)
		{
			this->bytes = new unsigned char[this->sizeInBytes];

			memset( this->bytes, 0, this->sizeInBytes );
		}

		explicit PlfLst( const vector<LSTF>& _rgLstf ) :  bytes(NULL), sizeInBytes(sizeof(short))
		{
			if ( !_rgLstf.empty() )
			{
				this->sizeInBytes += ( _rgLstf.size() * _rgLstf[0].Size() );
			}

			this->bytes = new unsigned char[this->sizeInBytes];

			memset( this->bytes, 0, this->sizeInBytes );

			FormatUtils::SetBytes( this->bytes, (short)_rgLstf.size() );

			for ( unsigned int i = 0; i < _rgLstf.size(); i++ )
			{
				memcpy( ( this->bytes + sizeof(short) + ( i * _rgLstf[i].Size() ) ), (unsigned char*)_rgLstf[i], _rgLstf[i].Size() );
			}
		}

		PlfLst( const PlfLst& _plfLst ) : bytes(NULL), sizeInBytes(_plfLst.sizeInBytes)
		{
			this->bytes = new unsigned char[this->sizeInBytes];

			memset( this->bytes, 0, this->sizeInBytes );

			memcpy( this->bytes, _plfLst.bytes, this->sizeInBytes );
		}

		virtual ~PlfLst()
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

		unsigned char* bytes;
		unsigned int sizeInBytes;
	};
}