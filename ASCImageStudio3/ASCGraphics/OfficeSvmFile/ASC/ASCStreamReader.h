#pragma once

#include "OpenOfficeBasic.h"
#include "tools/errcode.hxx"
#include "rtl/textenc.h"

#include "ASCBasic.h"




namespace SVMCore{

//\tools\inc\tools\stream.hxx
// read, write, create,... options
#define STREAM_READ 					0x0001	// allow read accesses
#define STREAM_WRITE					0x0002	// allow write accesses

#define COMPRESSMODE_FULL			(sal_uInt16)0xFFFF
#define COMPRESSMODE_NONE			(sal_uInt16)0x0000
#define COMPRESSMODE_ZBITMAP			(sal_uInt16)0x0001
#define COMPRESSMODE_NATIVE 			(sal_uInt16)0x0010

#define NUMBERFORMAT_INT_BIGENDIAN		(sal_uInt16)0x0000
#define NUMBERFORMAT_INT_LITTLEENDIAN	(sal_uInt16)0xFFFF

#define STREAM_SEEK_TO_BEGIN			0L
#define STREAM_SEEK_TO_END				ULONG_MAX

class SvStream
{
private: sal_uInt16 m_nCompressionMode;
private: sal_uInt32	nError;
private: rtl_TextEncoding	eStreamCharSet;


public: virtual bool Open( std::wstring sFilename ) = 0;
public: virtual int Read( void* pData, int nSize ) = 0;
public: virtual sal_Size Seek( sal_Size nFilePos ) = 0;
public: virtual sal_Size Tell() = 0;
public: virtual SvStream* CreateInstance() = 0;
public: virtual void SetBuffer( char* pcBuffer, sal_uInt32 nSize ) = 0;
public: virtual bool IsEof() = 0;
public: SvStream()
		{
			nError = SVSTREAM_OK;
			m_nCompressionMode = COMPRESSMODE_NONE;
			eStreamCharSet = RTL_TEXTENCODING_DONTKNOW;
		}
public: virtual ~SvStream(){}
public:SvStream& operator >> ( long& r )
{
	Read( &r, sizeof( long ) );
    return *this;
}

public:SvStream& operator >> ( short& r )
{
	Read( &r, sizeof( short ) );
    return *this;
}

public:SvStream& operator >> ( int& r )
{
	Read( &r, sizeof( int ) );
    return *this;
}

public:SvStream& operator>>( signed char& r )
{
	Read( &r, sizeof( signed char ) );
    return *this;
}
public:SvStream& operator>>( char& r )
{
	Read( &r, sizeof( char ) );
    return *this;
}

public:SvStream& operator>>( unsigned char& r )
{
	Read( &r, sizeof( unsigned char ) );
    return *this;
}
public:SvStream& operator>>( unsigned short& r )
{
	Read( &r, sizeof( unsigned short ) );
    return *this;
}
public:SvStream& operator>>( unsigned long&  r )
{
	Read( &r, sizeof( unsigned long ) );
    return *this;
}
public:SvStream& operator>>( float& r )
{
    Read( &r, sizeof( float ) );
    return *this;
}

public:SvStream& operator>>( double& r )
{
    Read( &r, sizeof( double ) );
    return *this;
}
public:SvStream& operator>>( wchar_t& r )
{
    Read( &r, sizeof( wchar_t ) );
    return *this;
}
public:SvStream& operator>>( ByteString& r )
{
	ReadByteString( r );
    return *this;
}
public:sal_uInt16 GetCompressMode()
{
    return m_nCompressionMode;
}
public:void SetCompressMode( sal_uInt16 nCompMode )
{
    m_nCompressionMode = nCompMode;
}
public: sal_uInt16 GetNumberFormatInt()
		{
			return NUMBERFORMAT_INT_LITTLEENDIAN;
		}
// -----------------------------------------------------------------------

public: SvStream& ReadByteString( String& rStr, rtl_TextEncoding eSrcCharSet )
{
    // read UTF-16 string directly from stream ?
    if (eSrcCharSet == RTL_TEXTENCODING_UNICODE)
    {
        sal_uInt32 nLen;
        operator>> (nLen);
        if (nLen)
        {
#ifdef AVS
            if (nLen > STRING_MAXLEN) {
                SetError(SVSTREAM_GENERALERROR);
                return *this;
            }
#endif
            sal_Unicode *pStr = rStr.AllocBuffer(nLen);
            //BOOST_STATIC_ASSERT(STRING_MAXLEN <= SAL_MAX_SIZE / 2);
            Read( pStr, nLen << 1 );
#ifdef AVS
            if (bSwap)
                for (sal_Unicode *pEnd = pStr + nLen; pStr < pEnd; pStr++)
                    SwapUShort((sal_uInt16&)(*pStr));
#endif
        }
        else
            rStr = String();

        return *this;
    }
	ByteString oTempString;
    ReadByteString( oTempString );
	rStr = String( oTempString, eSrcCharSet );
    return *this;
}

// -----------------------------------------------------------------------

public: SvStream& ReadByteString( ByteString& rStr )
{
    sal_uInt16 nLen = 0;
    operator>>( nLen );
    if( nLen )
    {
		char* pTmp = rStr.AllocBuffer( nLen );
        Read( pTmp, nLen );
    }
    else
		rStr = ByteString("");
    return *this;
}
public: sal_Size SeekRel( sal_sSize nPos )
{
    sal_Size nActualPos = Tell();

	if ( nPos >= 0 )
	{
		if ( SAL_MAX_SIZE - nActualPos > (sal_Size)nPos )
    		nActualPos += nPos;
	}
	else
	{
		sal_Size nAbsPos = (sal_Size)-nPos;
		if ( nActualPos >= nAbsPos )
			nActualPos -= nAbsPos;
	}
    return Seek( nActualPos );
}

public: void SetError( sal_uInt32 nErrorCode )
{
    if ( nError == SVSTREAM_OK )
        nError = nErrorCode;
}

public: sal_uInt32	GetError() const { return ERRCODE_TOERROR(nError); }
public: void	ResetError() { nError = SVSTREAM_OK; }
public: rtl_TextEncoding GetStreamCharSet() const { return eStreamCharSet; }
};

}//SVMCore