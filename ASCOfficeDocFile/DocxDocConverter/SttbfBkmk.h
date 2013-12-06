#pragma once

#include "STTB.h"

namespace ASCDocFileFormat
{
	class BookmarkName: public IOperand
	{
	public:

		explicit BookmarkName( const WCHAR* _bookmarkName ) : bytes(NULL), sizeInBytes(0)
		{
			if ( ( _bookmarkName != NULL ) && ( wcslen( _bookmarkName ) < 40 ) )
			{
				this->bookmarkName = wstring( _bookmarkName );

				this->sizeInBytes = ( sizeof(WCHAR) * wcslen( _bookmarkName ) );

				if ( this->sizeInBytes > 0 )
				{
					this->bytes = new byte[this->sizeInBytes];

					if ( this->bytes != NULL )
					{
						memset( this->bytes, 0, this->sizeInBytes );

						memcpy( this->bytes, (void*)_bookmarkName, this->sizeInBytes );
					}
				}
			}
		}

		BookmarkName( const BookmarkName& _bookmarkName ) : bookmarkName(_bookmarkName.bookmarkName), bytes(NULL), sizeInBytes(_bookmarkName.sizeInBytes)
		{
			if ( this->sizeInBytes > 0 )
			{
				this->bytes = new byte[this->sizeInBytes];

				if ( this->bytes != NULL )
				{
					memset( this->bytes, 0, this->sizeInBytes );

					memcpy( this->bytes, _bookmarkName.bytes, this->sizeInBytes );
				}
			}
		}

		wstring GetName() const
		{
			return this->bookmarkName;
		}

		virtual ~BookmarkName()
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
	private:

		wstring bookmarkName;
		byte* bytes;
		unsigned int sizeInBytes;
	};

	typedef STTB<BookmarkName> SttbfBkmk;
}