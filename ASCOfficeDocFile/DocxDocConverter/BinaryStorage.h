/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#pragma once

#include "../Common/Singleton.h"
#include "../pole/pole.h"

namespace ASCDocFileFormat
{
	class BinaryStorage
	{
		struct BinaryStorageData
		{
			unsigned char* data;
			unsigned int size;

			BinaryStorageData():
			data(NULL), size(0)
			{
			}

			BinaryStorageData( const unsigned char* _data, unsigned int _size ):
			data(NULL), size(0)
			{
				if ( ( _data != NULL ) && ( _size != NULL ) )
				{
					this->size = _size;

					this->data = new unsigned char[this->size];

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

					this->data = new unsigned char[this->size];

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
		POLE::Storage*	pIStorage;
		POLE::Stream*	pIStream;

		list<BinaryStorageData> data;
		unsigned long offset;
		unsigned long startOffset;
		bool ownStream;

	public:
		explicit BinaryStorage( unsigned long _startOffset = 0, POLE::Stream* _pIStream = NULL ):
		pIStream(_pIStream), pIStorage(NULL), offset(_startOffset), startOffset(_startOffset), ownStream(false)
		{
		}

		long BindToStorage( POLE::Storage* _pIStorage, const char* _stream_name )
		{
			HRESULT res = S_FALSE;

			pIStorage = _pIStorage;

			if ( pIStorage != NULL )
			{
				if ( ( pIStream != NULL ) && ( this->ownStream ) )
				{
					delete pIStream;
					pIStream = NULL;
				}

				pIStream = new POLE::Stream(pIStorage,_stream_name,true);

				if (pIStream == NULL)return S_FALSE;
				//res = this->pIStorage->OpenStream( _stream, NULL, ( STGM_READWRITE | STGM_DIRECT | STGM_SHARE_EXCLUSIVE ), NULL, &(this->pIStream) );
				//{
				//	res = this->pIStorage->CreateStream( _stream, ( STGM_READWRITE | STGM_DIRECT | STGM_SHARE_EXCLUSIVE ), NULL, NULL, &(this->pIStream) );
				//}
			}

			if ( SUCCEEDED( res ) )
			{
				this->ownStream = true;
			}

			return res;
		}

		virtual ~BinaryStorage()
		{
			if ( ( pIStream != NULL ) && ( ownStream ) )
			{
				delete pIStream;
				pIStream = NULL;
			}
		}

		unsigned long PushData( const unsigned char* _data, unsigned int _size )
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

			POLE::uint64 pos = startOffset;

			if ( pIStream != NULL )
			{
				for ( list<BinaryStorageData>::const_iterator iter = data.begin(); iter != data.end(); iter++ )
				{
					pIStream->seek( pos);
					writtenSize = pIStream->write( iter->data, iter->size );
					pos += iter->size;
				}
				pIStream->flush();
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
