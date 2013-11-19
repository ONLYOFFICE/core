#include "stdafx.h"

#include <stdio.h>
#include "MemoryUtils.h"
#include "FontFileBase.h"

//------------------------------------------------------------------------
// CFontFileBase
//------------------------------------------------------------------------

CFontFileBase::CFontFileBase(char *sFile, int nLen, BOOL bFreeFileData) 
{
	m_sFileData = m_sFile = (unsigned char *)sFile;
	m_nLen = nLen;
	m_bFreeFileData = bFreeFileData;
}

CFontFileBase::~CFontFileBase() 
{
	if ( m_bFreeFileData ) 
		MemUtilsFree( m_sFileData );
}

char *CFontFileBase::ReadFile(wchar_t *wsFileName, int *pnFileLen) 
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

int          CFontFileBase::GetS8    (int nPos, BOOL *pbSuccess) 
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

int          CFontFileBase::GetU8    (int nPos, BOOL *pbSuccess) 
{
	*pbSuccess = TRUE;
	if ( nPos < 0 || nPos >= m_nLen ) 
	{
		*pbSuccess = FALSE;
		return 0;
	}
	return m_sFile[ nPos ];
}

int          CFontFileBase::GetS16BE (int nPos, BOOL *pbSuccess) 
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

int          CFontFileBase::GetU16BE (int nPos, BOOL *pbSuccess) 
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

int          CFontFileBase::GetS32BE (int nPos, BOOL *pbSuccess) 
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

unsigned int CFontFileBase::GetU32BE (int nPos, BOOL *pbSuccess) 
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

unsigned int CFontFileBase::GetUVarBE(int nPos, int nSize, BOOL *pbSuccess) 
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

BOOL CFontFileBase::CheckRegion(int nPos, int nSize) 
{
	return (nPos >= 0 && nPos + nSize >= nPos && nPos + nSize <= m_nLen);
}
