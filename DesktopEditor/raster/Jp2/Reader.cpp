#include "Types.h"
#include "Reader.h"

namespace Jpeg2000
{
	inline void SWAP_USHORT(unsigned int& x)
	{
		unsigned short y = ((x << 8) | (x >> 8)); 
		x = y;
	}
	inline void SWAP_UINT(unsigned int& x)
	{
		unsigned int y = ((x >> 24) | (((x >> 16) & 0xff) << 8) | (((x >> 8) & 0xff) << 16) | ((x & 0xff) << 24)); 
		x = y;
	}
	//-------------------------------------------------------------------------------------------------------------------------------
	// Byte input-output (Stream IO)
	//-------------------------------------------------------------------------------------------------------------------------------
	static int           ByteIO_Tell(ByteIO *pStream)
	{
		return pStream->pCurPos - pStream->pStart;
	}
	static void          ByteIO_Seek(ByteIO *pStream, int nPos)
	{
		pStream->pCurPos = pStream->pStart + nPos;
	}
	static bool          ByteIO_ByteOut(ByteIO *pStream, unsigned char nValue)
	{
		if (pStream->pCurPos >= pStream->pEnd)
		{
			return false;
		}
		*pStream->pCurPos++ = nValue;
		return true;
	}
	static unsigned char ByteIO_ByteIn(ByteIO *pStream)
	{
		if (pStream->pCurPos >= pStream->pEnd || pStream->pCurPos < pStream->pStart)
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
		ByteIO *pStream = (ByteIO*)Malloc(sizeof(ByteIO));

		if (!pStream)
			return NULL;

		//	pStream->pInfo = pInfo;

		if (pBuffer && nLength)
		{
			pStream->nMode   = STREAM_READ;
			pStream->pBuffer = pBuffer;
			pStream->nLength = nLength;
		}
		else if (!pBuffer && !nLength && pInfo)
		{
			// Выделяем память для закодировнного изображения
			pStream->nMode = STREAM_WRITE;
			switch (pInfo->eCodecFormat)
			{
				case codecJ2K:
				pCodingParams = ((J2kCodestream*)pInfo->pJ2k)->pCodingParams;
				break;
				case codecJP2:
				pCodingParams = ((Jp2Stream*)pInfo->pJp2)->pJ2k->pCodingParams;
				break;
				default:
				Free(pStream);
				return NULL;
			}
			pStream->nLength = pCodingParams->nXTsiz * pCodingParams->nYTsiz * pCodingParams->nXTilesCount * pCodingParams->nYTilesCount * 4;
			pStream->pBuffer = (unsigned char *)Malloc(pStream->nLength);
			if (!pStream->pBuffer)
			{
				Free(pStream);
				return NULL;
			}
		}
		else
		{
			Free(pStream);
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
		if (pStream)
		{
			if (STREAM_WRITE == pStream->nMode)
			{
				Free(pStream->pBuffer);
			}
			Free(pStream);
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
		for (int nIndex = nLen - 1; nIndex >= 0; nIndex--)
		{
			if (!ByteIO_ByteOut(pStream, (unsigned char)((nValue >> (nIndex << 3)) & 0xff)))
				return 0;
		}
		return nLen;
	}
	unsigned int   ByteIO_Read(ByteIO *pStream, int nLen)
	{
		unsigned int nResult = 0;
		for (int nIndex = nLen - 1; nIndex >= 0; nIndex--)
		{
			nResult += ByteIO_ByteIn(pStream) << (nIndex << 3);
		}
		return nResult;
	}
	void           ByteIO_Skip(ByteIO *pStream, int nCount)
	{
		pStream->pCurPos += nCount;
	}
	//-------------------------------------------------------------------------------------------------------------------------------
	// CReaderStream
	//-------------------------------------------------------------------------------------------------------------------------------
	CReaderStream::CReaderStream(void)
	{
		m_pStream = NULL;
		Close();
	}
	CReaderStream::CReaderStream(void* pStream, int nSizeStream)
	{
		m_pStream = NULL;
		Open(pStream, nSizeStream);
	}
	CReaderStream::~CReaderStream(void)
	{
		Close();
	}
	bool         CReaderStream::Open(void* pStream, int nSizeStream)
	{
		Close();
		m_pStream = ByteIO_Open(NULL, (unsigned char *)pStream, nSizeStream);

		if (!m_pStream)
			return false;

		m_nSize = nSizeStream;
		return true;
	}
	void         CReaderStream::Close()
	{
		if (m_pStream)
			ByteIO_Close(m_pStream);

		m_pStream   = NULL;
		m_nPosition = 0;
		m_nSkiped   = 0;
		m_nSize     = 0;
	}
	void         CReaderStream::Skip(int nSize)
	{
		m_nSkiped += nSize;
	}
	unsigned int CReaderStream::Read(int nSize)
	{
		if (!m_pStream)
			return 0;

		if (m_nSkiped)
		{
			ByteIO_Skip(m_pStream, m_nSkiped);
			m_nPosition += m_nSkiped;
			m_nSkiped =0;
		}

		unsigned int unRes = ByteIO_Read(m_pStream, nSize);
		m_nPosition += nSize;
		return unRes;
	}
	void         CReaderStream::Seek(int nPosition)
	{
		if (!m_pStream)
			return;

		m_nSkiped = 0;
		ByteIO_Seek(m_pStream, nPosition);
		m_nPosition = nPosition;
	}
	unsigned int CReaderStream::Write(unsigned int nValue, int nLen)
	{
		if (m_nSkiped)
		{
			ByteIO_Skip(m_pStream, m_nSkiped);
			m_nPosition += m_nSkiped;
			m_nSkiped = 0;
		}

		unsigned int unRes = ByteIO_Write(m_pStream, nValue, nLen);
		m_nPosition += nLen;
		return unRes;
	}
	int          CReaderStream::Tell()
	{
		return (m_nPosition + m_nSkiped);
	}
	int          CReaderStream::GetLeftSize()
	{
		return m_nSize - (m_nPosition + m_nSkiped);
	}
	void*        CReaderStream::GetOwner()
	{
		if (!m_pStream)
			return NULL;

		return m_pStream->pBuffer;
	}
	void         CReaderStream::Read(BYTE* pData, int nSize)
	{
		if (!m_pStream)
			return;

		if (m_nSkiped)
		{
			ByteIO_Skip(m_pStream, m_nSkiped);
			m_nPosition += m_nSkiped;
			m_nSkiped = 0;
		}

		memcpy(pData, m_pStream->pBuffer + m_nPosition, nSize);

		m_pStream->pCurPos += nSize;
		m_nPosition += nSize;
	}
	//-------------------------------------------------------------------------------------------------------------------------------
	// CReaderFile
	//-------------------------------------------------------------------------------------------------------------------------------
	CReaderFile::CReaderFile(void)
	{
		Close();
	}
	CReaderFile::CReaderFile(const std::wstring& wsFileName, long lMode)
	{
		Open(wsFileName, lMode);
	}
	CReaderFile::~CReaderFile(void)
	{
		Close();
	}
	bool         CReaderFile::Open(const std::wstring& wsFileName, int nSizeStream)
	{
		Close();

		bool bResult = true;
		if (nSizeStream > 0)
		{
			bResult = m_oFile.OpenFile(wsFileName);
		}
		else
		{
			bResult = m_oFile.CreateFileW(wsFileName);
		}

		if (!bResult)
		{
			m_oFile.CloseFile();
			return false;
		}

		m_pFile = m_oFile.GetFileNative();
		m_nSize = m_oFile.GetFileSize();

		return true;
	}
	void         CReaderFile::Close()
	{
		m_oFile.CloseFile();
		m_pFile     = NULL;
		m_nPosition = 0;
		m_nSkiped   = 0;
		m_nSize     = 0;
	}
	void         CReaderFile::Skip(int nSize)
	{
		if (!m_pFile)
			return;

		fseek(m_pFile, nSize, SEEK_CUR);
		m_nPosition += nSize;
	}
	unsigned int CReaderFile::Read(int nSize)
	{
		if (!m_pFile)
			return 0;

		if (m_nSkiped)
		{
			fseek(m_pFile, m_nSkiped, SEEK_CUR);
			m_nPosition += m_nSkiped;
			m_nSkiped = 0;
		}

		unsigned int unRes = 0;
        fread(&unRes, 1, (std::min)(nSize, 4), m_pFile);

		if (2 == nSize)
		{
			SWAP_USHORT(unRes);
		}
		else if (4 == nSize)
		{
			SWAP_UINT(unRes);
		}

		m_nPosition += nSize;

		return unRes;
	}
	void         CReaderFile::Seek(int nPosition)
	{
		if (!m_pFile)
			return;

		m_nSkiped = 0;
		fseek(m_pFile, nPosition, SEEK_SET);
		m_nPosition = nPosition;
	}
	unsigned int CReaderFile::Write(unsigned int nValue, int nLen)
	{		
		if (!m_pFile)
			return 0;

        fwrite(&nValue, 1, (std::min)(4, nLen), m_pFile);
		m_nPosition += nLen;
		return 0;
	}
	int          CReaderFile::Tell()
	{
		return (m_nPosition + m_nSkiped);
	}
	int          CReaderFile::GetLeftSize()
	{
		return m_nSize - (m_nPosition + m_nSkiped);
	}
	void*        CReaderFile::GetOwner()
	{
		return m_pFile;
	}
	void         CReaderFile::Read(BYTE* pData, int nSize)
	{
		if (!m_pFile)
			return;

		if (m_nSkiped)
		{
			fseek(m_pFile, m_nSkiped, SEEK_CUR);
			m_nPosition += m_nSkiped;
			m_nSkiped = 0;
		}

		fread(pData, 1, nSize, m_pFile);
		m_nPosition += nSize;
	}
}
