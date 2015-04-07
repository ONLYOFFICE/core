#pragma once

#include "LVL.h"

namespace ASCDocFileFormat
{
	class LFOLVL: public IOperand
	{
	private:
		unsigned char* bytes;
		unsigned int sizeInBytes;

	public:

		LFOLVL() : bytes(NULL), sizeInBytes(0)
		{
			this->sizeInBytes = ( sizeof(int) + sizeof(unsigned int) );

			if ( this->sizeInBytes != 0 )
			{
				this->bytes = new unsigned char[this->sizeInBytes];

				if ( this->bytes != NULL )
				{
					memset( this->bytes, 0, this->sizeInBytes );
				}
			}
		}

		explicit LFOLVL( int _iStartAt, unsigned char _iLvl, bool _fStartAt, grfhic _grfhic, const LVL* _lvl = NULL ) : bytes(NULL), sizeInBytes(0)
		{
			this->sizeInBytes = ( sizeof(_iStartAt) + sizeof(unsigned int) );

			if ( _lvl != NULL )
			{
				this->sizeInBytes += _lvl->Size();
			}

			if ( this->sizeInBytes != 0 )
			{
				this->bytes = new unsigned char[this->sizeInBytes];

				if ( this->bytes != NULL )
				{
					memset( this->bytes, 0, this->sizeInBytes );

					int iStartAt = 0;

					if ( _iStartAt > 0x7FFF )
					{
						iStartAt = 0x7FFF; 
					}
					else if ( _iStartAt < 0 )
					{
						iStartAt = 0;  
					}
					else
					{
						iStartAt = _iStartAt;  
					}

					FormatUtils::SetBytes( this->bytes, iStartAt );

					unsigned int flags = 0;
					unsigned int fStartAt = 0;
					unsigned int fFormatting = 0;
					unsigned int grfhicValue = (unsigned int)(unsigned char)_grfhic;
					grfhicValue <<= 6;

					( _iLvl <= 0x08 ) ? ( flags = _iLvl ) : ( flags = 0x08 );
					( _fStartAt ) ? ( fStartAt = 0x10 ) : ( fStartAt = 0 );
					( _lvl != NULL ) ? ( fFormatting = 0x20 ) : ( fFormatting = 0 );

					flags |= ( fStartAt | fFormatting | grfhicValue );

					FormatUtils::SetBytes( ( this->bytes + sizeof(iStartAt) ), flags );

					if ( _lvl != NULL )
					{
						memcpy( ( this->bytes + sizeof(iStartAt) + sizeof(flags) ), (unsigned char*)(*_lvl), _lvl->Size() );
					}
				}
			}
		}

		LFOLVL( const LFOLVL& _lFOLVL ) : bytes(NULL), sizeInBytes(_lFOLVL.sizeInBytes)
		{
			this->bytes = new unsigned char[this->sizeInBytes];

			if ( this->bytes != NULL )
			{
				memset( this->bytes, 0, this->sizeInBytes );

				memcpy( this->bytes, _lFOLVL.bytes, this->sizeInBytes );
			}
		}

		virtual ~LFOLVL()
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
	};
}