#pragma once

#include "Constants.h"

namespace ASCDocFileFormat
{
	class EmptyExtraData
	{
	public:
		unsigned int Size() const
		{
			return 0;
		}

		operator byte*() const
		{
			return (byte*)NULL;
		}
	};

	template<class D, class E = EmptyExtraData> class STTB: public IOperand
	{
	private:
		byte* bytes;
		unsigned int sizeInBytes;

	public:
		STTB() : bytes(NULL), sizeInBytes(0)
		{
		}

		explicit STTB( bool fExtend, const vector<D>* _data, const vector<E>* _extraData = NULL ) : bytes(NULL), sizeInBytes(0)
		{
			//!!!TODO!!!
			if ( ( _data == NULL ) || ( ( _data != NULL ) && ( _extraData != NULL ) && ( _data->size() != _extraData->size() ) ) )
			{
				return;
			}

			if ( fExtend )
			{
				this->sizeInBytes += sizeof(WCHAR); //fExtend
				this->sizeInBytes += ( 2 * _data->size() ); //cchData
			}
			else
			{
				this->sizeInBytes += ( 1 * _data->size() ); //cchData  
			}

			for ( vector<D>::const_iterator iter = _data->begin(); iter != _data->end(); iter++ )
			{
				this->sizeInBytes += iter->Size();
			}

			if ( _data->size() < 0xFFFF )
			{
				this->sizeInBytes += sizeof(unsigned short); //cData
			}
			else
			{
				this->sizeInBytes += sizeof(int); //cData
			}

			this->sizeInBytes += 2; //cbExtra

			if ( _extraData != NULL )
			{
				for ( vector<E>::const_iterator iter = _extraData->begin(); iter != _extraData->end(); iter++ )
				{
					this->sizeInBytes += iter->Size();
				}  
			}

			this->bytes = new byte[this->sizeInBytes];
			memset( this->bytes, 0, this->sizeInBytes );

			unsigned int offset = 0;

			if ( fExtend )
			{
				FormatUtils::SetBytes( ( this->bytes + offset ), (short)(0xFFFF) );
				offset += sizeof(short);
			}

			if ( _data->size() < 0xFFFF )
			{
				FormatUtils::SetBytes( ( this->bytes + offset ), (short)(_data->size()) );
				offset += sizeof(unsigned short);
			}
			else
			{
				FormatUtils::SetBytes( ( this->bytes + offset ), (int)(_data->size()) );
				offset += sizeof(int);
			}

			if ( _extraData != NULL )
			{
				FormatUtils::SetBytes( ( this->bytes + offset ), (unsigned short)(_extraData->front().Size()) );
			}
			else
			{
				FormatUtils::SetBytes( ( this->bytes + offset ), (unsigned short)0 );
			}

			offset += sizeof(unsigned short);  

			if ( fExtend )
			{
				for ( unsigned int i = 0; i < _data->size(); i++ )
				{
					FormatUtils::SetBytes( ( this->bytes + offset ), (unsigned short)( _data->at( i ).Size() / 2 ) );
					offset += sizeof(unsigned short);
					memcpy( ( this->bytes + offset ), (byte*)(_data->at( i )), _data->at( i ).Size() );
					offset += _data->at( i ).Size();

					if ( _extraData != NULL )
					{
						memcpy( ( this->bytes + offset ), (byte*)(_extraData->at( i )), _extraData->at( i ).Size() );
						offset += _extraData->at( i ).Size();  
					}
				}
			}
			else
			{
				for ( unsigned int i = 0; i < _data->size(); i++ )
				{
					FormatUtils::SetBytes( ( this->bytes + offset ), (byte)( _data->at( i ).Size()) );
					offset += sizeof(byte);
					memcpy( ( this->bytes + offset ), (byte*)(_data->at( i )), _data->at( i ).Size() );
					offset += _data->at( i ).Size();

					if ( _extraData != NULL )
					{
						memcpy( ( this->bytes + offset ), (byte*)(_extraData->at( i )), _extraData->at( i ).Size() );
						offset += _extraData->at( i ).Size();  
					}
				}
			}
		}

		STTB( const STTB& _sttb ) : bytes(NULL), sizeInBytes(_sttb.sizeInBytes)
		{
			this->bytes = new byte[this->sizeInBytes];

			if ( this->bytes != NULL )
			{
				memset( this->bytes, 0, this->sizeInBytes );

				memcpy( this->bytes, _sttb.bytes, this->sizeInBytes );
			}
		}

		bool operator == ( const STTB& _sttb )
		{
			return ( ( this->sizeInBytes == _sttb.sizeInBytes ) && 
				( memcmp( this->bytes, _sttb.bytes, this->sizeInBytes ) == 0 ) );
		}

		bool operator != ( const STTB& _sttb )
		{
			return !( this->operator == ( _sttb ) );
		}

		STTB& operator = ( const STTB& _sttb )
		{
			if ( *this != _sttb )
			{
				RELEASEARRAYOBJECTS (bytes);

				this->sizeInBytes = _sttb.sizeInBytes;

				this->bytes = new byte[this->sizeInBytes];

				if ( this->bytes != NULL )
				{
					memcpy( this->bytes, _sttb.bytes, this->sizeInBytes );
				}  
			}

			return *this;
		}

		virtual ~STTB()
		{
			RELEASEARRAYOBJECTS( bytes );
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