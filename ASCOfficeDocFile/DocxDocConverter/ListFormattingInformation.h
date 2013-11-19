#pragma once

#include "PlfLst.h"
#include "LVL.h"

namespace AVSDocFileFormat
{
	class ListFormattingInformation: public IOperand
	{
	private:
		byte* bytes;
		unsigned int sizeInBytes;
		unsigned int listFormattingInformationSize;
		unsigned int listLevelsSize;

	public:

		ListFormattingInformation() : bytes(NULL), sizeInBytes(0), listFormattingInformationSize(0), listLevelsSize(0)
		{
		}

		explicit ListFormattingInformation( const PlfLst& _plfLst, const vector<LVL>& _lvls ) : bytes(NULL), sizeInBytes(0), listFormattingInformationSize(0), listLevelsSize(0)
		{
			this->listFormattingInformationSize = _plfLst.Size();

			for ( vector<LVL>::const_iterator iter = _lvls.begin(); iter != _lvls.end(); iter++ )
			{
				this->listLevelsSize += iter->Size();
			}

			this->sizeInBytes = ( this->listFormattingInformationSize + this->listLevelsSize );

			if ( this->sizeInBytes != 0 )
			{
				this->bytes = new byte[this->sizeInBytes];

				if ( this->bytes != NULL )
				{
					memset( this->bytes, 0, this->sizeInBytes );

					memcpy( this->bytes, (byte*)_plfLst, _plfLst.Size() );

					unsigned int offset = _plfLst.Size();

					for ( vector<LVL>::const_iterator iter = _lvls.begin(); iter != _lvls.end(); iter++ )
					{
						memcpy( ( this->bytes + offset ), (byte*)(*iter), iter->Size() );
						offset += iter->Size();
					}
				}
			}
		}

		ListFormattingInformation( const ListFormattingInformation& _lfi ) : bytes(NULL), sizeInBytes(_lfi.sizeInBytes), listFormattingInformationSize(_lfi.listFormattingInformationSize), listLevelsSize(_lfi.listLevelsSize)
		{
			this->bytes = new byte[this->sizeInBytes];

			if ( this->bytes != NULL )
			{
				memset( this->bytes, 0, this->sizeInBytes );

				memcpy( this->bytes, _lfi.bytes, this->sizeInBytes );
			}
		}

		bool operator == ( const ListFormattingInformation& _lfi )
		{
			return ( ( this->sizeInBytes == _lfi.sizeInBytes ) &&
				( this->listFormattingInformationSize == _lfi.listFormattingInformationSize ) &&
				( this->listLevelsSize == _lfi.listLevelsSize ) &&
				( memcmp( this->bytes, _lfi.bytes, this->sizeInBytes ) == 0 ) );
		}

		bool operator != ( const ListFormattingInformation& _lfi )
		{
			return !( this->operator == ( _lfi ) );
		}

		ListFormattingInformation& operator = ( const ListFormattingInformation& _lfi )
		{
			if ( *this != _lfi )
			{
				RELEASEARRAYOBJECTS (bytes);

				this->sizeInBytes = _lfi.sizeInBytes;
				this->listFormattingInformationSize = _lfi.listFormattingInformationSize;
				this->listLevelsSize = _lfi.listLevelsSize;

				this->bytes = new byte[this->sizeInBytes];

				if ( this->bytes != NULL )
				{
					memcpy( this->bytes, _lfi.bytes, this->sizeInBytes );
				}  
			}

			return *this;
		}

		virtual ~ListFormattingInformation()
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

		unsigned int ListFormattingInformationSize() const
		{
			return this->listFormattingInformationSize;
		}

		unsigned int ListLevelsSize() const
		{
			return this->listLevelsSize;
		}
	};
}