#ifndef _FONT_FILE_BASE_H
#define _FONT_FILE_BASE_H

#include <stdio.h>
#include "MemoryUtils.h"

//------------------------------------------------------------------------

typedef void (*FontFileOutputFunc)(void *pStream, char *sData, int nLen);

//------------------------------------------------------------------------
// CFontFileBase
//------------------------------------------------------------------------

class CFontFileBase 
{
public:

	virtual ~CFontFileBase()
	{
		if ( m_bFreeFileData ) 
			MemUtilsFree( m_sFileData );
	}

protected:

	CFontFileBase(char *sFile, int nLen, BOOL bFreeFileData) 
	{
		m_sFileData = m_sFile = (unsigned char *)sFile;
		m_nLen = nLen;
		m_bFreeFileData = bFreeFileData;
		m_nPos = 0;
	}

	void         Reset()
	{
		m_nPos = 0;
	}
	static char *ReadFile(wchar_t *wsFileName, int *pnFileLen) 
	{
		FILE *pFile;

		if ( !( pFile = _wfopen( wsFileName, _T("rb") ) ) ) 	
			return NULL;

		fseek( pFile, 0, SEEK_END );
		int nLen = (int)ftell( pFile );
		fseek( pFile, 0, SEEK_SET );
		char *sBuffer = (char *)MemUtilsMalloc( nLen );
		if ( (int)fread( sBuffer, 1, nLen, pFile) != nLen ) 
		{
			MemUtilsFree( sBuffer );
			fclose( pFile );
			return NULL;
		}

		fclose( pFile );
		*pnFileLen = nLen;
		return sBuffer;
	}

	// S = signed / U = unsigned
	// 8/16/32/Var = word length, in bytes
	// BE = big endian
	int          GetS8    (int nPos, BOOL *pbSuccess) 
	{
		*pbSuccess = TRUE;

		if ( nPos < 0 || nPos >= m_nLen ) 
		{
			*pbSuccess = FALSE;
			return 0;
		}
		int nRes = m_sFile[ nPos ];
		if ( nRes & 0x80 ) 
			nRes |= ~0xff;
		return nRes;
	}

	int          GetU8    (int nPos, BOOL *pbSuccess) 
	{
		*pbSuccess = TRUE;
		if ( nPos < 0 || nPos >= m_nLen ) 
		{
			*pbSuccess = FALSE;
			return 0;
		}
		return m_sFile[ nPos ];
	}

	int          GetS16BE (int nPos, BOOL *pbSuccess) 
	{
		*pbSuccess = TRUE;

		if ( nPos < 0 || nPos + 1 >= m_nLen ) 
		{
			*pbSuccess = FALSE;
			return 0;
		}
		int nRes = m_sFile[nPos];
		nRes = (nRes << 8) + m_sFile[ nPos + 1 ];
		if ( nRes & 0x8000 )
			nRes |= ~0xffff;
		return nRes;
	}

	int          GetU16BE (int nPos, BOOL *pbSuccess) 
	{
		*pbSuccess = TRUE;

		if ( nPos < 0 || nPos + 1 >= m_nLen) 
		{
			*pbSuccess = FALSE;
			return 0;
		}
		int nRes = m_sFile[ nPos ];
		nRes = (nRes << 8) + m_sFile[ nPos + 1 ];
		return nRes;
	}

	int          GetS32BE (int nPos, BOOL *pbSuccess) 
	{
		*pbSuccess = TRUE;

		if ( nPos < 0 || nPos + 3 >= m_nLen ) 
		{
			*pbSuccess = FALSE;
			return 0;
		}
		int nRes = m_sFile[ nPos ];
		nRes = (nRes << 8) + m_sFile[nPos + 1];
		nRes = (nRes << 8) + m_sFile[nPos + 2];
		nRes = (nRes << 8) + m_sFile[nPos + 3];
		if ( nRes & 0x80000000 ) 
			nRes |= ~0xffffffff;

		return nRes;
	}

	unsigned int GetU32BE (int nPos, BOOL *pbSuccess) 
	{
		*pbSuccess = TRUE;

		if ( nPos < 0 || nPos + 3 >= m_nLen ) 
		{
			*pbSuccess = FALSE;
			return 0;
		}
		unsigned int nRes = m_sFile[nPos];
		nRes = (nRes << 8) + m_sFile[nPos + 1];
		nRes = (nRes << 8) + m_sFile[nPos + 2];
		nRes = (nRes << 8) + m_sFile[nPos + 3];
		return nRes;
	}
	unsigned int GetU32LE (int nPos, BOOL *pbSuccess) 
	{
		*pbSuccess = TRUE;

		if ( nPos < 0 || nPos + 3 >= m_nLen ) 
		{
			*pbSuccess = FALSE;
			return 0;
		}
		unsigned int nRes = m_sFile[nPos + 3];
		nRes = (nRes << 8) + m_sFile[nPos + 2];
		nRes = (nRes << 8) + m_sFile[nPos + 1];
		nRes = (nRes << 8) + m_sFile[nPos + 0];
		return nRes;
	}
	unsigned int GetUVarBE(int nPos, int nSize, BOOL *pbSuccess) 
	{
		*pbSuccess = TRUE;

		if ( nPos < 0 || nPos + nSize > m_nLen ) 
		{
			*pbSuccess = FALSE;
			return 0;
		}
		unsigned int nRes = 0;
		for ( int nIndex = 0; nIndex < nSize; ++nIndex ) 
			nRes = (nRes << 8) + m_sFile[nPos + nIndex];

		return nRes;
	}

	BOOL         CheckRegion(int nPos, int nSize) 
	{
		return (nPos >= 0 && nPos + nSize >= nPos && nPos + nSize <= m_nLen);
	}
	int          ReadS8   (BOOL *pbSuccess)
	{
		return GetS8( m_nPos++, pbSuccess );
	}
	int          ReadU8   (BOOL *pbSuccess)
	{
		return GetU8( m_nPos++, pbSuccess );
	}
	unsigned int ReadU32BE(BOOL *pbSuccess)
	{
		unsigned int unResult = GetU32BE( m_nPos, pbSuccess );
		m_nPos += 4;
		return unResult;
	}
	unsigned int ReadU32LE(BOOL *pbSuccess)
	{
		unsigned int unResult = GetU32LE( m_nPos, pbSuccess );
		m_nPos += 4;
		return unResult;
	}
	int          Read(void* pDestBuffer, int nSize)
	{
		if ( m_nPos + nSize >= m_nLen )
			nSize = m_nLen - m_nPos - 1;

		memcpy( pDestBuffer, (m_sFile + m_nPos), nSize );
		m_nPos += nSize;

		return nSize;
	}

protected:

	unsigned char *m_sFileData;
	unsigned char *m_sFile;
	int            m_nLen;
	BOOL           m_bFreeFileData;
	int            m_nPos;

};

#endif /* _FONT_FILE_BASE_H */
