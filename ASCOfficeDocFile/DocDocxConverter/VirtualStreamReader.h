#pragma once

#include "IBinaryReader.h"

class VirtualStreamReader : public IBinaryReader
{
public:
	VirtualStreamReader (IStream* stream, ULONG position = 0) : stream(NULL), position(0), nRd(0)
	{
		this->stream = stream;
		this->position = position;

		nSeekNext.HighPart = 0;
		nSeekNext.LowPart = 0;

		if ( this->stream != NULL )
		{
			LARGE_INTEGER nSeekPos = { position, 0 };
			this->stream->Seek( nSeekPos, STREAM_SEEK_SET, NULL );
		}
	}

	virtual ~VirtualStreamReader()
	{
	}

	virtual unsigned short ReadUInt16()
	{
		unsigned short rdUShort = 0;

		if ( this->stream != NULL )
		{
			LARGE_INTEGER nSeekPos = { position, 0 };
			this->stream->Seek( nSeekPos, STREAM_SEEK_SET, &(this->nSeekNext) );
			this->stream->Read( &rdUShort, sizeof( rdUShort ), &(this->nRd) );
			position += sizeof( rdUShort );
		}

		return rdUShort;
	}

	virtual short ReadInt16()
	{
		short rdShort = 0;

		if ( this->stream != NULL )
		{
			LARGE_INTEGER nSeekPos = { position, 0 };
			this->stream->Seek( nSeekPos, STREAM_SEEK_SET, &(this->nSeekNext) );
			this->stream->Read( &rdShort, sizeof( rdShort ), &(this->nRd) );
			position += sizeof( rdShort );
		}

		return rdShort;
	}

	virtual int ReadInt32()
	{
		int rdInt = 0;

		if ( this->stream != NULL )
		{
			LARGE_INTEGER nSeekPos = { position, 0 };
			this->stream->Seek( nSeekPos, STREAM_SEEK_SET, &(this->nSeekNext) );
			this->stream->Read( &rdInt, sizeof( rdInt ), &(this->nRd) );
			position += sizeof( rdInt );
		}

		return rdInt;
	}

	virtual unsigned int ReadUInt32()
	{
		int rdUInt = 0;

		if ( this->stream != NULL )
		{
			LARGE_INTEGER nSeekPos = { position, 0 };
			this->stream->Seek( nSeekPos, STREAM_SEEK_SET, &(this->nSeekNext) );
			this->stream->Read( &rdUInt, sizeof( rdUInt ), &(this->nRd) );
			position += sizeof( rdUInt );
		}

		return rdUInt;
	}

	virtual byte ReadByte()
	{
		byte rdByte = 0;

		if ( this->stream != NULL )
		{
			LARGE_INTEGER nSeekPos = { position, 0 };
			this->stream->Seek( nSeekPos, STREAM_SEEK_SET, &(this->nSeekNext) );
			this->stream->Read( &rdByte, sizeof( rdByte ), &(this->nRd) );
			position += sizeof( rdByte );
		}

		return rdByte;
	}

	virtual byte* ReadBytes( unsigned int count, bool isResultNeeded )
	{
		byte *rdBytes = NULL;

		if ( ( this->stream != NULL ) && ( count > 0 ) )
		{
			rdBytes = new byte[count];

			if ( rdBytes != NULL )
			{
				LARGE_INTEGER nSeekPos = { position, 0 };

				this->stream->Seek( nSeekPos, STREAM_SEEK_SET, &(this->nSeekNext) );
				this->stream->Read( rdBytes, sizeof( byte )*count, &(this->nRd) );

				position += sizeof( byte )*count;

				if (!isResultNeeded)
				{
					RELEASEARRAYOBJECTS(rdBytes);
				}
			}
		}

		return rdBytes;
	}

	virtual unsigned long GetPosition() const
	{
		return this->position;
	}

	virtual unsigned long GetSize() const
	{
		unsigned long size = 0;

		if ( this->stream != NULL )
		{
			STATSTG stg;

			this->stream->Stat( &stg, STATFLAG_NONAME );

			size = stg.cbSize.LowPart;
		}

		return size;
	}

	virtual int Seek( int offset, STREAM_SEEK origin )
	{
		if ( this->stream != NULL )
		{
			LARGE_INTEGER nSeekPos = { offset, 0 };
			this->stream->Seek( nSeekPos, origin, &(this->nSeekNext) );
			this->position = offset;
			return this->nSeekNext.LowPart;
		}
		else
		{
			return 0;
		}
	}

	std::wstring ReadXst()
	{
		std::wstring wstrResult( _T( "" ) );
		if (stream)
		{
			int cchSize = 2;
			byte* cch = this->ReadBytes( cchSize, true );

			int xstzSize = FormatUtils::BytesToInt16( cch, 0, cchSize ) * 2;      
			byte* xstz = ReadBytes(xstzSize, true);

			FormatUtils::GetSTLCollectionFromBytes<wstring>( &wstrResult, xstz, xstzSize, ENCODING_UNICODE );

			RELEASEARRAYOBJECTS(xstz);
			RELEASEARRAYOBJECTS(cch);
		}

		return wstrResult;
	}

	/// Read a length prefixed Unicode string from the given stream.
	/// The string must have the following structure:
	/// byte 1 - 4: Character count (cch)
	/// byte 5 - (cch*2)+4: Unicode characters terminated by \0
	std::wstring ReadLengthPrefixedUnicodeString()
	{
		wstring result;

		int cch = this->ReadInt32();

		if ( cch > 0 )
		{
			//dont read the terminating zero
			byte* stringBytes = this->ReadBytes( ( cch * 2 ), true );

			FormatUtils::GetSTLCollectionFromBytes<wstring>( &result, stringBytes, ( ( cch * 2 ) - 2 ), ENCODING_UNICODE );

			RELEASEARRAYOBJECTS( stringBytes );
		}

		return result;
	}

	/// Read a length prefixed ANSI string from the given stream.
	/// The string must have the following structure:
	/// byte 1-4: Character count (cch)
	/// byte 5-cch+4:   ANSI characters terminated by \0
	std::wstring ReadLengthPrefixedAnsiString()
	{
		wstring result;

		int cch = this->ReadInt32();

		if ( cch > 0 )
		{
			//dont read the terminating zero
			byte* stringBytes = this->ReadBytes( cch, true );

			FormatUtils::GetSTLCollectionFromBytes<wstring>( &result, stringBytes, ( cch - 1 ), ENCODING_WINDOWS_1251 );

			RELEASEARRAYOBJECTS( stringBytes );
		}

		return result;
	}

private:

	ULONG nRd;
	ULARGE_INTEGER nSeekNext;
	ULONG position;
	IStream* stream;
};


#ifdef _DEBUG

#include <AtlFile.h>

inline static void DumpBuffer (CString strFile, VirtualStreamReader oReader, unsigned int iSeekTo, unsigned int iBuffSize)
{
	CAtlFile oFile;
	if (SUCCEEDED(oFile.Create(strFile,GENERIC_WRITE, FILE_SHARE_WRITE, CREATE_ALWAYS)))
	{
		if (oReader.Seek(iSeekTo, STREAM_SEEK_SET))
		{
			BYTE* pBuffer = oReader.ReadBytes (iBuffSize, true);
			if (pBuffer)
			{
				DWORD nFileBuff	=	0L;
				oFile.Write (pBuffer, iBuffSize, &nFileBuff );
				RELEASEOBJECT(pBuffer);
			}

			oFile.Close ();
		}
	}
}

#endif
