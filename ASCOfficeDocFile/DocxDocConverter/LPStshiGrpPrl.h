#pragma once

#include "Prl.h"

namespace ASCDocFileFormat
{
	class LPStshiGrpPrl: public IOperand
	{
	public:

		LPStshiGrpPrl() : bytes(NULL), sizeInBytes(0)
		{
		}

		explicit LPStshiGrpPrl( const vector<Prl>& _grpprl ) : bytes(NULL), sizeInBytes(0), grpprl(_grpprl)
		{
			this->sizeInBytes = sizeof(int);

			for ( vector<Prl>::const_iterator iter = this->grpprl.begin(); iter != this->grpprl.end(); iter++ )
			{
				this->sizeInBytes += iter->Size();
			}

			this->bytes = new unsigned char[this->sizeInBytes];

			if ( this->bytes != NULL )
			{
				memset( this->bytes, 0, this->sizeInBytes );

				FormatUtils::SetBytes( this->bytes, (int)( this->sizeInBytes - sizeof(int) ) );

				unsigned char* prlBytes = NULL;
				unsigned long prlSize = 0;
				unsigned int prlPrevSize = 0;

				for ( vector<Prl>::iterator iter = this->grpprl.begin(); iter != this->grpprl.end(); iter++ )
				{
					prlBytes = iter->GetBytes( &prlSize );

					if ( prlBytes != NULL )
					{
						memcpy( ( this->bytes + sizeof(int) + prlPrevSize ), prlBytes, prlSize );
						prlPrevSize += prlSize;

						RELEASEARRAYOBJECTS (prlBytes);
					}
				}
			}
		}

		LPStshiGrpPrl( const LPStshiGrpPrl& _lPStshiGrpPrl ) : bytes(NULL), sizeInBytes(_lPStshiGrpPrl.sizeInBytes), grpprl(_lPStshiGrpPrl.grpprl)
		{
			this->bytes = new unsigned char[this->sizeInBytes];

			if ( this->bytes != NULL )
			{
				memset( this->bytes, 0, this->sizeInBytes );
				memcpy( this->bytes, _lPStshiGrpPrl.bytes, this->sizeInBytes );
			}
		}

		virtual ~LPStshiGrpPrl()
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

		vector<Prl> grpprl;

		unsigned char* bytes;
		unsigned int sizeInBytes;
	};
}