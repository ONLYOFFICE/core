#pragma once
#include "SHA1/sha1.h"
#include "ASCUniversalConverterUtils.h"

#define ZLIB_WINAPI
#define CODEPAGE_ISSUE_FIX
#include "zlib.h"

#define BUFFER_ENCODE_READ		2048 //должно быть больше чем 1040
#define BUFFER_ENCODE_WRITE		8196
#define BUFFER_DECODE_READ		2048
#define BUFFER_DECODE_WRITE		8196


CStringA Strip( CStringA sInput )
{
	CStringA sResult = sInput;
	sResult.Remove( ' ' );
	sResult.Remove( 0x20 );
	sResult.Remove( 0x09 );
	sResult.Remove( 0x0D );
	sResult.Remove( 0x0A );
	return sResult;
}

//XOR инвертируема, поэтому функция защифровывает и дешифрует
bool CryptFile( CString sSourceFile, CString sDestFilename, CStringA sInit, bool bEncode, bool bIDPF = true  )
{
	byte* mask = NULL;
	int  masklength = 0;
	int  headerLen = 0;

	if( true == bIDPF )
	{//IDPF
		//получаем SHA-1 hash для sInit
		SHA1 oSha1;
		oSha1.Reset();
		sInit = Strip( sInit );
		oSha1.Input( sInit, sInit.GetLength() );
		unsigned  int  Hash[5]; 
		if( false == oSha1.Result( (unsigned  int*)Hash ) )
			return false;
		//преобразуем в массив байтов
		mask = new byte[ 20 ];
		masklength = 20;
		headerLen = 1040;
		for( int i = 0; i < 5 ; i++ )
		{
			mask[ i * 4 + 0 ] = (Hash[i] & 0xff000000) >> 24;
			mask[ i * 4 + 1 ] = (Hash[i] & 0x00ff0000) >> 16;
			mask[ i * 4 + 2 ] = (Hash[i] & 0x0000ff00) >> 8;
			mask[ i * 4 + 3 ] = (Hash[i] & 0x000000ff) >> 0;
		}
	}
	else
	{//ADOBE
		if( -1 != sInit.Find( "urn:uuid:" ) )
			sInit.Delete( 0, 9 );
		CAtlArray<byte> aTempBytes;

		int acc = 0;
		int len = sInit.GetLength();
		for (int i = 0; i < len; i++)
		{
			char c = sInit[i];
			int n;
			if ('0' <= c && c <= '9')
				n = c - '0';
			else if ('a' <= c && c <= 'f')
				n = c - ('a' - 10);
			else if ('A' <= c && c <= 'F')
				n = c - ('A' - 10);
			else
				continue;
			if (acc == 0)
			{
				acc = 0x100 | (n << 4);
			}
			else
			{
				aTempBytes.Add(acc | n);
				acc = 0;
			}
		}
		if (aTempBytes.GetCount() != 16)
			return false;
		masklength = aTempBytes.GetCount();
		mask = new byte[ masklength ];
		headerLen = 1024;
		for( int i  = 0;  i < masklength; i++ )
			mask[i] = aTempBytes[i];
	}

	BYTE* byteReadBuffer = NULL;
	DWORD dwReadBuffSize = NULL;
	if( true == bEncode )
	{
		byteReadBuffer = new BYTE[ BUFFER_ENCODE_READ ];
		dwReadBuffSize = BUFFER_ENCODE_READ;
	}
	else
	{
		byteReadBuffer = new BYTE[ BUFFER_DECODE_READ ];
		dwReadBuffSize = BUFFER_DECODE_READ;
	}

	//открываем для чтения
	HANDLE hFileRead = ::CreateFile( sSourceFile, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );
	DWORD dwBytesRead = 0;
	//открываем для записи
	HANDLE hFileWrite = ::CreateFile( sDestFilename, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0 );
	DWORD dwBytesWrite = 0;
	if( INVALID_HANDLE_VALUE == hFileRead || INVALID_HANDLE_VALUE == hFileWrite )
		return false;
	//Некомпрессованный буфер
	BYTE* outBuff = NULL;
	DWORD dwOutBuffSize = NULL;
	if( true == bEncode )
	{
		outBuff = new BYTE[ BUFFER_ENCODE_WRITE ];
		dwOutBuffSize = BUFFER_ENCODE_WRITE;
	}
	else
	{
		outBuff = new BYTE[ BUFFER_DECODE_WRITE ];
		dwOutBuffSize = BUFFER_DECODE_WRITE;
	}
	

	::ReadFile( hFileRead, byteReadBuffer, dwReadBuffSize, &dwBytesRead, NULL );

	long dwRead = 0, dwWritten = 0;
	bool bFirst = true;
	bool bError = false;
	int nRes = 0;

    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;

	if( true == bEncode )
		nRes = deflateInit2( &strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, -15, MAX_MEM_LEVEL, Z_DEFAULT_STRATEGY );
	else
		nRes = inflateInit2( &strm, -15 );

    if (nRes != Z_OK)
        return false;

	nRes = 0;
	do
	{
		if( true == bEncode )
		{
			strm.next_in = byteReadBuffer;
			strm.avail_in = dwBytesRead;
			do
			{
				strm.avail_out = dwOutBuffSize;
				strm.next_out = outBuff;

				nRes = deflate( &strm, Z_FULL_FLUSH );
				if( S_OK != nRes && Z_STREAM_END != nRes )
					return false;

				long nBytesToWrite = dwOutBuffSize - strm.avail_out;
				if( headerLen > 0 )
				{
					int nMinSize = headerLen;
					if( nMinSize > nBytesToWrite )
						nMinSize = nBytesToWrite;
					headerLen -= nMinSize;

					for( int i = 0; i < nMinSize; i++ )
						outBuff[ i ] = outBuff[ i ] ^ mask[ i % masklength ];
				}
				DWORD dwBytesWrittenInFile = 0;
				::WriteFile( hFileWrite, outBuff , nBytesToWrite, &dwBytesWrittenInFile, NULL );
			}
			while( strm.avail_in != 0 );

			::ReadFile( hFileRead, byteReadBuffer ,dwReadBuffSize, &dwBytesRead, NULL );
		}
		else
		{
			strm.next_in = byteReadBuffer;
			strm.avail_in = dwBytesRead;
			if( headerLen > 0 )
			{
				int nMinSize = headerLen;
				if( nMinSize > dwBytesRead )
					nMinSize = dwBytesRead;
				headerLen -= nMinSize;
				for( int i = 0; i < nMinSize; i++ )
					byteReadBuffer[ i ] = byteReadBuffer[ i ] ^ mask[ i % masklength ];
			}
			do
			{
				strm.avail_out = dwOutBuffSize;
				strm.next_out = outBuff;

				nRes = inflate(&strm, Z_NO_FLUSH);

				if( S_OK != nRes && Z_STREAM_END != nRes )
					return false;

				long nBytesToWrite = dwOutBuffSize - strm.avail_out;
				DWORD dwBytesWrittenInFile = 0;
				::WriteFile( hFileWrite, outBuff , nBytesToWrite, &dwBytesWrittenInFile, NULL );
			}
			while( strm.avail_in != 0 );

			::ReadFile( hFileRead, byteReadBuffer ,dwReadBuffSize, &dwBytesRead, NULL );
		}
	}
	while( Z_STREAM_END != nRes && dwBytesRead > 0 );

	if( true == bEncode )
		deflateEnd(&strm);
	else
		inflateEnd(&strm);

	RELEASEARRAYOBJECTS( byteReadBuffer );
	RELEASEARRAYOBJECTS( outBuff );
	RELEASEARRAYOBJECTS( mask );
	CloseHandle( hFileRead );
	CloseHandle( hFileWrite );
	return true;
}