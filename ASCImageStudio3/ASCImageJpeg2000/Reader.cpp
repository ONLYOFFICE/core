#include "StdAfx.h"
#include "Types.h"
#include "Reader.h"
//-------------------------------------------------------------------------------------------------------------------------------
// Byte input-output (Stream IO)
//-------------------------------------------------------------------------------------------------------------------------------

static int           ByteIO_Tell   (ByteIO *pStream) 
{
	return pStream->pCurPos - pStream->pStart;
}

static void          ByteIO_Seek   (ByteIO *pStream, int nPos) 
{
	pStream->pCurPos = pStream->pStart + nPos;
}
static bool          ByteIO_ByteOut(ByteIO *pStream, unsigned char nValue) 
{
	if ( pStream->pCurPos >= pStream->pEnd ) 
	{
		return false;
	}
	*pStream->pCurPos++ = nValue;
	return true;
}

static unsigned char ByteIO_ByteIn (ByteIO *pStream) 
{
	if ( pStream->pCurPos >= pStream->pEnd  || pStream->pCurPos< pStream->pStart) 
	{
		return 0;
	}
	return *pStream->pCurPos++;
}

//-------------------------------------------------------------------------------------------------------------------------------
// Основные функции
//-------------------------------------------------------------------------------------------------------------------------------
ByteIO*        ByteIO_Open(PCommon pInfo, unsigned char *pBuffer, int nLength)
{
	CodingParams *pCodingParams = NULL;
	ByteIO *pStream = (ByteIO*)Malloc(HEAP_ZERO_MEMORY,  sizeof(ByteIO) );

	if( !pStream ) 
		return NULL;
	
//	pStream->pInfo = pInfo;

	if( pBuffer && nLength ) 
	{
		pStream->nMode   = STREAM_READ;
		pStream->pBuffer = pBuffer;
		pStream->nLength = nLength;
	}
	else if( !pBuffer && !nLength && pInfo ) 
	{
		// Выделяем память для закодировнного изображения
		pStream->nMode = STREAM_WRITE;
		switch( pInfo->eCodecFormat ) 
		{
			case codecJ2K:
				pCodingParams = ((J2kCodestream*)pInfo->pJ2k)->pCodingParams;
				break;
			case codecJP2:
				pCodingParams = ((Jp2Stream*)pInfo->pJp2)->pJ2k->pCodingParams;
				break;
			default:
				RELEASEHEAP( pStream );
				return NULL;
		}
		pStream->nLength = pCodingParams->nXTsiz * pCodingParams->nYTsiz * pCodingParams->nXTilesCount * pCodingParams->nYTilesCount * 4;
		pStream->pBuffer = (unsigned char *)Malloc(HEAP_ZERO_MEMORY,  pStream->nLength );
		if( !pStream->pBuffer ) 
		{
			RELEASEHEAP( pStream );
			return NULL;
		}
	}
	else 
	{
		RELEASEHEAP( pStream );
		return NULL;
	}

	// Инициализируем поток
	pStream->pStart  = pStream->pBuffer;
	pStream->pEnd    = pStream->pBuffer + pStream->nLength;
	pStream->pCurPos = pStream->pBuffer;

	return pStream;
}

void           ByteIO_Close(ByteIO *pStream) 
{
	if( pStream ) 
	{
		if( STREAM_WRITE == pStream->nMode ) 
		{
			RELEASEHEAP( pStream->pBuffer );
		}
		RELEASEHEAP( pStream );
	}
}
int            ByteIO_CountBytesLeft(ByteIO *pStream) 
{
	return pStream->pEnd - pStream->pCurPos;
}
unsigned char *ByteIO_GetCurPos(ByteIO *pStream) 
{
	return pStream->pCurPos;
}


unsigned int   ByteIO_Write(ByteIO *pStream, unsigned int nValue, int nLen) 
{
	for ( int nIndex = nLen - 1; nIndex >= 0; nIndex-- ) 
	{
		if( !ByteIO_ByteOut( pStream, (unsigned char) ((nValue >> (nIndex << 3)) & 0xff ) ) )
			return 0;
	}
	return nLen;
}


unsigned int   ByteIO_Read(ByteIO *pStream, int nLen) 
{
	unsigned int nResult = 0;
	for ( int nIndex = nLen - 1; nIndex >= 0; nIndex-- ) 
	{
		nResult += ByteIO_ByteIn( pStream ) << ( nIndex << 3 );
	}
	return nResult;
}

void           ByteIO_Skip(ByteIO *pStream, int nCount) 
{
	pStream->pCurPos += nCount;
}


CReaderStream::CReaderStream(void)
{
	m_oStream=NULL;
	Close();
}
CReaderStream::CReaderStream(void* sStream,long lSizeStream)
{
	m_oStream=NULL;

	Open(sStream,lSizeStream);
}
CReaderStream::~CReaderStream(void)
{
	Close();
}

bool CReaderStream::Open(void* sStream,long lSizeStream)
{
	Close();
	m_oStream=ByteIO_Open(NULL, (unsigned char *)sStream, lSizeStream);

	if (!m_oStream) return false;

	m_llSize = lSizeStream;

	return true;
}
void CReaderStream::Close()
{
	if (m_oStream)ByteIO_Close(m_oStream);

	m_oStream = NULL;

	m_llPosition =0;
	m_lSkiped=0;

	m_llSize=0;
}

void CReaderStream::Skip(long size)
{
	m_lSkiped+=size;
}
DWORD CReaderStream::Read(long size)
{
	if (!m_oStream)return 0;
	if (m_lSkiped)
	{
		ByteIO_Skip(m_oStream,m_lSkiped);
		m_llPosition +=m_lSkiped;
		m_lSkiped =0;
	}
	DWORD res = ByteIO_Read(m_oStream, size) ;
	m_llPosition +=size;

	return res;
}

void CReaderStream::Seek(__int64 position)
{
	if (!m_oStream)return;

	m_lSkiped=0;
	ByteIO_Seek(m_oStream,position);
	m_llPosition = position;
}

unsigned int   CReaderStream::Write(unsigned int nValue, int nLen)
{
	if (m_lSkiped)
	{
		ByteIO_Skip(m_oStream,m_lSkiped);
		m_llPosition +=m_lSkiped;
		m_lSkiped =0;
	}

	unsigned int res = ByteIO_Write(m_oStream,  nValue,  nLen) ;
	m_llPosition+=nLen;

	return res;
}
int CReaderStream::Tell()
{
	return (m_llPosition+m_lSkiped);
}
int CReaderStream::GetLeftSize()
{
	return m_llSize- (m_llPosition+m_lSkiped);
}

void* CReaderStream::GetOwner()
{
	if (!m_oStream)return NULL;
	return m_oStream->pBuffer;
}

void  CReaderStream::Read(BYTE* Data,long Size)
{
	if (!m_oStream)return;
	if (m_lSkiped)
	{
		ByteIO_Skip(m_oStream,m_lSkiped);
		m_llPosition +=m_lSkiped;
		m_lSkiped =0;
	}

	memcpy(Data,m_oStream->pBuffer + m_llPosition,Size);

	m_oStream->pCurPos+=Size;
	m_llPosition +=Size;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////


CReaderFile::CReaderFile(void)
{
	m_oFile.m_h=NULL;
	Close();
}
CReaderFile::CReaderFile(CString sFileName,long lMode)
{
	m_oFile.m_h=NULL;

	Open((void*)(LPCTSTR)sFileName,lMode);
}
CReaderFile::~CReaderFile(void)
{
	Close();
}

bool CReaderFile::Open(void* sStream,long lSizeStream)
{
	Close();
	
	HRESULT hr = 0;
	
	CString sFileName =CString((LPCTSTR)sStream);
	if (lSizeStream>0)
	{
		hr = m_oFile.Create(sFileName, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN);
	}
	else
	{		
		hr = m_oFile.Create(sFileName, GENERIC_WRITE, FILE_SHARE_WRITE, CREATE_ALWAYS, FILE_FLAG_SEQUENTIAL_SCAN);
	}

	if (!m_oFile.m_h || hr) return false;

	ULONGLONG ullSize=0;
	m_oFile.GetSize(ullSize);
	m_llSize = ullSize;

	return true;
}
void CReaderFile::Close()
{
	if (m_oFile.m_h)
		m_oFile.Close();

	m_oFile.m_h = NULL;

	m_llPosition =0;
	m_lSkiped=0;

	m_llSize=0;
}

void CReaderFile::Skip(long size)
{
	m_lSkiped+=size;
}
DWORD CReaderFile::Read(long size)
{
	if (!m_oFile.m_h)return 0;
	if (m_lSkiped)
	{	
		m_oFile.Seek(m_lSkiped,SEEK_CUR);
		m_llPosition +=m_lSkiped;
		m_lSkiped =0;
	}
	DWORD res = 0;
	HRESULT hr = m_oFile.Read(&res, min(size,4)) ;
	if (size==2)
	{
		SWAP_INT1(res);
	}
	if (size==4)
	{
		SWAP_LONG1(res);
	}
	m_llPosition +=size;

	return res;
}

void CReaderFile::Seek(__int64 position)
{
	if (!m_oFile.m_h)return;

	m_lSkiped=0;
	m_oFile.Seek(position,SEEK_SET);
	m_llPosition = position;
}

unsigned int   CReaderFile::Write(unsigned int nValue, int nLen)
{
	//if (m_lSkiped)
	//{
	//	ByteIO_Skip(m_oStream,m_lSkiped);
	//	m_llPosition +=m_lSkiped;
	//	m_lSkiped =0;
	//}

	HRESULT hr  = m_oFile.Write(&nValue,min(4, nLen)) ;
	m_llPosition+=nLen;

	return 0;
}
int CReaderFile::Tell()
{
	return (m_llPosition + m_lSkiped);
}
int CReaderFile::GetLeftSize()
{
	return m_llSize - (m_llPosition+m_lSkiped);
}

void* CReaderFile::GetOwner()
{
	if (!m_oFile.m_h)return NULL;
	return (void*)m_oFile.m_h;
}

void  CReaderFile::Read(BYTE* Data,long Size)
{
	if (!m_oFile.m_h)return;
	if (m_lSkiped)
	{
		m_oFile.Seek(m_lSkiped,SEEK_CUR);
		m_llPosition +=m_lSkiped;
		m_lSkiped =0;
	}
	m_oFile.Read(Data,Size);
	m_llPosition +=Size;
}
