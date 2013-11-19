#pragma once

#include "..\Common\Singleton.h"

namespace AVSDocFileFormat
{
	class BinaryStorage
	{
		struct BinaryStorageData
		{
			byte* data;
			unsigned int size;

			BinaryStorageData():
			data(NULL), size(0)
			{
			}

			BinaryStorageData( const byte* _data, unsigned int _size ):
			data(NULL), size(0)
			{
				if ( ( _data != NULL ) && ( _size != NULL ) )
				{
					this->size = _size;

					this->data = new byte[this->size];

					if ( this->data != NULL )
					{
						memset( this->data, 0, this->size );

						memcpy( this->data, _data, this->size );
					}
				}
			}

			BinaryStorageData( const BinaryStorageData& _binaryStorageData ):
			data(NULL), size(0)
			{
				if ( ( _binaryStorageData.data != NULL ) && ( _binaryStorageData.size != NULL ) )
				{
					this->size = _binaryStorageData.size;

					this->data = new byte[this->size];

					if ( this->data != NULL )
					{
						memset( this->data, 0, this->size );

						memcpy( this->data, _binaryStorageData.data, this->size );
					}
				}
			}

			~BinaryStorageData()
			{
				RELEASEARRAYOBJECTS (data);
			}
		};

	protected:
		IStorage* pIStorage;
		IStream* pIStream;

		list<BinaryStorageData> data;
		unsigned long offset;
		unsigned long startOffset;
		bool ownStream;

	public:
		explicit BinaryStorage( unsigned long _startOffset = 0, IStream* _pIStream = NULL ):
		pIStream(_pIStream), pIStorage(NULL), offset(_startOffset), startOffset(_startOffset), ownStream(false)
		{
		}

		long BindToStorage( IStorage* _pIStorage, const wchar_t* _stream )
		{
			HRESULT res = S_FALSE;

			this->pIStorage = _pIStorage;

			if ( this->pIStorage != NULL )
			{
				if ( ( this->pIStream != NULL ) && ( this->ownStream ) )
				{
					this->pIStream->Release();
					this->pIStream = NULL;
				}

				res = this->pIStorage->OpenStream( _stream, NULL, ( STGM_READWRITE | STGM_DIRECT | STGM_SHARE_EXCLUSIVE ), NULL, &(this->pIStream) );

				if ( FAILED( res ) )
				{
					res = this->pIStorage->CreateStream( _stream, ( STGM_READWRITE | STGM_DIRECT | STGM_SHARE_EXCLUSIVE ), NULL, NULL, &(this->pIStream) );
				}
			}

			if ( SUCCEEDED( res ) )
			{
				this->ownStream = true;
			}

			return res;
		}

		virtual ~BinaryStorage()
		{
			if ( ( this->pIStream != NULL ) && ( this->ownStream ) )
			{
				this->pIStream->Release();
				this->pIStream = NULL;
			}
		}

		unsigned long PushData( const byte* _data, unsigned int _size )
		{
			this->data.push_back( BinaryStorageData( _data, _size ) );

			unsigned long dataOffset = this->offset;

			this->offset += _size;

			return dataOffset;
		}

		unsigned long ClearData()
		{
			this->data.clear();

			this->offset = this->startOffset;

			return this->offset;
		}

		long WriteData()
		{
			long res = S_OK;
			ULONG writtenSize;

			LARGE_INTEGER pos = { 0, 0 };

			pos.HighPart = 0;
			pos.LowPart = this->startOffset;

			if ( this->pIStream != NULL )
			{
				for ( list<BinaryStorageData>::const_iterator iter = this->data.begin(); iter != this->data.end(); iter++ )
				{
					res = this->pIStream->Seek( pos, STREAM_SEEK_SET, NULL );
					res = this->pIStream->Write( iter->data, iter->size, &writtenSize );
					pos.LowPart += iter->size;
				}
			}
			else
			{
				res = S_FALSE;
			}

			return res;  
		}
	};

	class BinaryStorageSingleton: public Singleton<BinaryStorageSingleton>, public BinaryStorage
	{
	protected:
		BinaryStorageSingleton(): BinaryStorage() {};
		virtual ~BinaryStorageSingleton() {};
		friend class Singleton<BinaryStorageSingleton>;
	};
}