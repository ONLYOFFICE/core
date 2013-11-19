#pragma once

#include "LFOLVL.h"

namespace AVSDocFileFormat
{
	class LFOData: public IOperand
	{
	private:
		byte* bytes;
		unsigned int sizeInBytes;

	public:

		LFOData() : bytes(NULL), sizeInBytes(0)
		{
			this->sizeInBytes = sizeof(unsigned int);

			if ( this->sizeInBytes != 0 )
			{
				this->bytes = new byte[this->sizeInBytes];

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
				this->bytes = new byte[this->sizeInBytes];

				if ( this->bytes != NULL )
				{
					memset( this->bytes, 0, this->sizeInBytes );

					FormatUtils::SetBytes( this->bytes, _cp );

					unsigned int offset = 0;

					for ( vector<LFOLVL>::const_iterator iter = _rgLfoLvl.begin(); iter != _rgLfoLvl.end(); iter++ )
					{
						memcpy( ( this->bytes + sizeof(_cp) + offset ), (byte*)(*iter), iter->Size() );

						offset += iter->Size();
					}
				}
			}
		}

		LFOData( const LFOData& _lFOData ) : bytes(NULL), sizeInBytes(_lFOData.sizeInBytes)
		{
			this->bytes = new byte[this->sizeInBytes];

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
	};
}