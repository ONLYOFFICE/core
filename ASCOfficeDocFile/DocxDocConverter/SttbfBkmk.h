#pragma once

#include "STTB.h"

namespace ASCDocFileFormat
{
	class BookmarkName: public IOperand
	{
	public:

		explicit BookmarkName( const wchar_t* _bookmarkName ) : bytes(NULL), sizeInBytes(0)
		{
			if ( ( _bookmarkName != NULL ) && ( wcslen( _bookmarkName ) < 40 ) )
			{
				this->bookmarkName = wstring( _bookmarkName );

				this->sizeInBytes = ( sizeof(wchar_t) * wcslen( _bookmarkName ) );

				if ( this->sizeInBytes > 0 )
				{
					this->bytes = new unsigned char[this->sizeInBytes];

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
				this->bytes = new unsigned char[this->sizeInBytes];

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

		wstring bookmarkName;
		unsigned char* bytes;
		unsigned int sizeInBytes;
	};

	typedef STTB<BookmarkName> SttbfBkmk;
}