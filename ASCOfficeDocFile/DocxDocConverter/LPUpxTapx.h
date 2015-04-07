#pragma once

#include "Prl.h"

namespace ASCDocFileFormat
{
	class UpxTapx : public IOperand
	{
	public:

		UpxTapx() : bytes(NULL), sizeInBytes(0), sizeInBytesWithoutPadding(0)
		{

		}

		UpxTapx( const vector<Prl>& _grpprlTapx ) : grpprlTapx(_grpprlTapx), bytes(NULL), sizeInBytes(0), sizeInBytesWithoutPadding(0)
		{
			for ( vector<Prl>::const_iterator iter = this->grpprlTapx.begin(); iter != this->grpprlTapx.end(); iter++ )
			{
				this->sizeInBytesWithoutPadding += iter->Size();
			}

			this->sizeInBytes = this->sizeInBytesWithoutPadding;

			if ( this->sizeInBytes > 0 )
			{
				//The padding to be an even length
				if ( this->sizeInBytes % 2 != 0 )
				{
					this->sizeInBytes++;
				}

				this->bytes = new unsigned char[this->sizeInBytes];

				if ( this->bytes != NULL )
				{
					memset( this->bytes, 0, this->sizeInBytes );

					unsigned char* prlBytes = NULL;
					unsigned long prlSize = 0;
					unsigned int prlPrevSize = 0;

					for ( vector<Prl>::iterator iter = this->grpprlTapx.begin(); iter != this->grpprlTapx.end(); iter++ )
					{
						prlBytes = iter->GetBytes( &prlSize );

						if ( prlBytes != NULL )
						{
							memcpy( ( this->bytes + prlPrevSize ), prlBytes, prlSize );
							prlPrevSize += prlSize;

							RELEASEARRAYOBJECTS (prlBytes);
						}
					}
				}
			}
		}

		UpxTapx( const UpxTapx& _upxTapx ):
		grpprlTapx(_upxTapx.grpprlTapx), bytes(NULL), sizeInBytes(_upxTapx.sizeInBytes),
			sizeInBytesWithoutPadding(_upxTapx.sizeInBytesWithoutPadding)
		{
			this->bytes = new unsigned char[this->sizeInBytes];

			if ( this->bytes != NULL )
			{
				memset( this->bytes, 0, this->sizeInBytes );
				memcpy( this->bytes, _upxTapx.bytes, this->sizeInBytes );
			}
		}

		virtual ~UpxTapx()
		{
			RELEASEARRAYOBJECTS (bytes);
		}

		UpxTapx& operator = ( const UpxTapx& _upxTapx )
		{
			if ( this != &_upxTapx )
			{
				RELEASEARRAYOBJECTS (bytes);

				this->grpprlTapx = _upxTapx.grpprlTapx;
				this->sizeInBytes = _upxTapx.sizeInBytes;
				this->sizeInBytesWithoutPadding = _upxTapx.sizeInBytesWithoutPadding;

				this->bytes = new unsigned char[this->sizeInBytes];

				if ( this->bytes != NULL )
				{
					memcpy( this->bytes, _upxTapx.bytes, this->sizeInBytes );
				}  
			}

			return *this;
		}

		const vector<Prl> GetProperties() const
		{
			return this->grpprlTapx;
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

		unsigned int SizeWithoutPadding() const
		{
			return this->sizeInBytesWithoutPadding;
		}
	private:

		vector<Prl> grpprlTapx;

		unsigned char* bytes;
		unsigned int sizeInBytes;
		unsigned int sizeInBytesWithoutPadding;
	};

	class LPUpxTapx : public IOperand
	{	
	public:

		LPUpxTapx () : upxTapx(), bytes(NULL), sizeInBytes(0), sizeInBytesWithoutPadding(0)
		{
			Init();
		}

		LPUpxTapx (const UpxTapx& _upxTapx) : upxTapx(_upxTapx), bytes(NULL), sizeInBytes(0), sizeInBytesWithoutPadding(0)
		{
			Init();
		}

		LPUpxTapx (const LPUpxTapx& _lPUpxTapx) : upxTapx(_lPUpxTapx.upxTapx), bytes(NULL), sizeInBytes(_lPUpxTapx.sizeInBytes), sizeInBytesWithoutPadding(_lPUpxTapx.sizeInBytesWithoutPadding)
		{
			bytes = new unsigned char[sizeInBytes];

			if ( bytes != NULL )
			{
				memset( bytes, 0, sizeInBytes );
				memcpy( bytes, _lPUpxTapx.bytes, sizeInBytes );
			}
		}

		bool operator == (const LPUpxTapx& _lPUpxTapx)
		{
			return ( ( this->sizeInBytes == _lPUpxTapx.sizeInBytes ) &&	( this->sizeInBytesWithoutPadding == _lPUpxTapx.sizeInBytesWithoutPadding ) && ( memcmp( this->bytes, _lPUpxTapx.bytes, this->sizeInBytes ) == 0 ) );
		}

		bool operator != (const LPUpxTapx& _lPUpxTapx)
		{
			return !( this->operator == ( _lPUpxTapx ) );
		}

		LPUpxTapx& operator = (const LPUpxTapx& _lPUpxTapx)
		{
			if ( this != &_lPUpxTapx )
			{
				RELEASEARRAYOBJECTS (bytes);

				upxTapx						=	_lPUpxTapx.upxTapx;
				sizeInBytes					=	_lPUpxTapx.sizeInBytes;
				sizeInBytesWithoutPadding	=	_lPUpxTapx.sizeInBytesWithoutPadding;

				bytes						=	new unsigned char[sizeInBytes];

				if ( bytes != NULL )
				{
					memcpy ( bytes, _lPUpxTapx.bytes, sizeInBytes );
				}  
			}

			return *this;
		}

		virtual ~LPUpxTapx()
		{
			RELEASEARRAYOBJECTS (bytes);
		}

		const vector<Prl> GetProperties() const
		{
			return upxTapx.GetProperties();
		}

		virtual operator unsigned char*() const
		{
			return bytes;
		}

		virtual operator const unsigned char*() const
		{
			return (const unsigned char*)bytes;
		}

		virtual unsigned int Size() const
		{
			return sizeInBytes;
		}

		unsigned int SizeWithoutPadding() const
		{
			return sizeInBytesWithoutPadding;
		}


	private: 

		void Init()
		{
			this->sizeInBytes = ( sizeof(unsigned short) + this->upxTapx.Size() );
			this->sizeInBytesWithoutPadding = ( sizeof(unsigned short) + this->upxTapx.SizeWithoutPadding() );

			this->bytes = new unsigned char[this->sizeInBytes];

			if ( this->bytes != NULL )
			{
				memset( this->bytes, 0, this->sizeInBytes );

				FormatUtils::SetBytes( this->bytes, (unsigned short)this->upxTapx.SizeWithoutPadding() );

				memcpy( ( this->bytes + sizeof(unsigned short) ), (unsigned char*)this->upxTapx, this->upxTapx.Size() );
			}
		}
	private:

		UpxTapx upxTapx;

		unsigned char* bytes;
		unsigned int sizeInBytes;
		unsigned int sizeInBytesWithoutPadding;
	};
}