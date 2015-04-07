#pragma once

#include "LFOLVL.h"

namespace ASCDocFileFormat
{
	class LFOData: public IOperand
	{
	private:
		unsigned char* bytes;
		unsigned int sizeInBytes;

	public:

		LFOData() : bytes(NULL), sizeInBytes(0)
		{
			this->sizeInBytes = sizeof(unsigned int);

			if ( this->sizeInBytes != 0 )
			{
				this->bytes = new unsigned char[this->sizeInBytes];

				if ( this->bytes != NULL )
				{
					memset( this->bytes, 0xFF, this->sizeInBytes );
				}
			}
		}

		explicit LFOData( unsigned int _cp, const vector<LFOLVL>& _rgLfoLvl ) : bytes(NULL), sizeInBytes(0)
		{
			this->sizeInBytes = sizeof(_cp);

			for ( vector<LFOLVL>::const_iterator iter = _rgLfoLvl.begin(); iter != _rgLfoLvl.end(); iter++ )
			{
				this->sizeInBytes += iter->Size();  
			}

			if ( this->sizeInBytes != 0 )
			{
				this->bytes = new unsigned char[this->sizeInBytes];

				if ( this->bytes != NULL )
				{
					memset( this->bytes, 0, this->sizeInBytes );

					FormatUtils::SetBytes( this->bytes, _cp );

					unsigned int offset = 0;

					for ( vector<LFOLVL>::const_iterator iter = _rgLfoLvl.begin(); iter != _rgLfoLvl.end(); iter++ )
					{
						memcpy( ( this->bytes + sizeof(_cp) + offset ), (unsigned char*)(*iter), iter->Size() );

						offset += iter->Size();
					}
				}
			}
		}

		LFOData( const LFOData& _lFOData ) : bytes(NULL), sizeInBytes(_lFOData.sizeInBytes)
		{
			this->bytes = new unsigned char[this->sizeInBytes];

			if ( this->bytes != NULL )
			{
				memset( this->bytes, 0, this->sizeInBytes );

				memcpy( this->bytes, _lFOData.bytes, this->sizeInBytes );
			}
		}

		virtual ~LFOData()
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