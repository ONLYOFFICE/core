#include "stdafx.h"

#include <stdlib.h>
#include <stdio.h>
#include "MemoryUtils.h"
#include "StringExt.h"
#include "SMathExt.h"
#include "SGlyphLibFontEngine.h"
#include "SFontFile.h"
#include "SFontFileID.h"
#include "SFont.h"
#include "SFontEngine.h"


//-------------------------------------------------------------------------------------------------------------------------------
// SFontEngine
//-------------------------------------------------------------------------------------------------------------------------------

SFontEngine::SFontEngine(BOOL bEnableGlyphLibrary, BOOL bAntiAliasing) 
{
	for ( int nIndex = 0; nIndex < sfontCacheSize; ++nIndex ) 
	{
		m_pFontCache[nIndex] = NULL;
	}

	if ( bEnableGlyphLibrary ) 
	{
		m_pGlyphLibEngine = SGlyphLibFontEngine::Init( bAntiAliasing );
	} 
	else 
	{
		m_pGlyphLibEngine = NULL;
	}
}

SFontEngine::~SFontEngine() 
{
	for ( int nIndex = 0; nIndex < sfontCacheSize; ++nIndex ) 
	{
		if ( m_pFontCache[nIndex] ) 
		{
			delete m_pFontCache[nIndex];
		}
	}

	if ( m_pGlyphLibEngine ) 
	{
		delete m_pGlyphLibEngine;
	}
}

SFontFile *SFontEngine::GetFontFile(SFontFileID *pFontFileID) 
{
	for ( int nIndex = 0; nIndex < sfontCacheSize; ++nIndex ) 
	{
		if ( m_pFontCache[nIndex] ) 
		{
			SFontFile *pFontFile = m_pFontCache[nIndex]->GetFontFile();
			if ( pFontFile && pFontFile->GetFontFileID()->Matches( pFontFileID ) ) 
			{
				return pFontFile;
			}
		}
	}
	return NULL;
}

SFontFile *SFontEngine::LoadType1Font(SFontFileID *pFontFileID, wchar_t *wsFileName, BOOL bDeleteFile, char **ppEncoding) 
{
	SFontFile *pFontFile = NULL;
	if ( !pFontFile && m_pGlyphLibEngine ) 
	{
		pFontFile = m_pGlyphLibEngine->LoadType1Font( pFontFileID, wsFileName, bDeleteFile, ppEncoding );
	}

	return pFontFile;
}

SFontFile *SFontEngine::LoadType1CFont(SFontFileID *pFontFileID, wchar_t *wsFileName, BOOL bDeleteFile, char **ppEncoding) 
{
	SFontFile *pFontFile = NULL;
	if ( !pFontFile && m_pGlyphLibEngine ) 
	{
		pFontFile = m_pGlyphLibEngine->LoadType1CFont( pFontFileID, wsFileName, bDeleteFile, ppEncoding );
	}

	return pFontFile;
}

SFontFile *SFontEngine::LoadOpenTypeT1CFont(SFontFileID *pFontFileID, wchar_t *wsFileName, BOOL bDeleteFile, char **ppEncoding) 
{
	SFontFile *pFontFile = NULL;

	if ( !pFontFile && m_pGlyphLibEngine ) 
	{
		pFontFile = m_pGlyphLibEngine->LoadOpenTypeT1CFont( pFontFileID, wsFileName, bDeleteFile, ppEncoding );
	}

	return pFontFile;
}

SFontFile *SFontEngine::LoadCIDFont(SFontFileID *pFontFileID, wchar_t *wsFileName, BOOL bDeleteFile) 
{
	SFontFile *pFontFile = NULL;

	if ( !pFontFile && m_pGlyphLibEngine ) 
	{
		pFontFile = m_pGlyphLibEngine->LoadCIDFont( pFontFileID, wsFileName, bDeleteFile );
	}

	return pFontFile;
}

SFontFile *SFontEngine::LoadOpenTypeCFFFont(SFontFileID *pFontFileID, wchar_t *wsFileName, BOOL bDeleteFile) 
{
	SFontFile *pFontFile = NULL;

	if ( !pFontFile && m_pGlyphLibEngine ) 
	{
		pFontFile = m_pGlyphLibEngine->LoadOpenTypeCFFFont( pFontFileID, wsFileName, bDeleteFile );
	}
	
	return pFontFile;
}

SFontFile *SFontEngine::LoadTrueTypeFont(SFontFileID *pFontFileID, wchar_t *wsFileName, BOOL bDeleteFile, unsigned short *pCodeToGID, int nCodeToGIDLen, wchar_t *wsTempFolder) 
{
	SFontFile *pFontFile = NULL;
	if ( !pFontFile && m_pGlyphLibEngine ) 
	{
		pFontFile = m_pGlyphLibEngine->LoadTrueTypeFont( pFontFileID, wsFileName, bDeleteFile, pCodeToGID, nCodeToGIDLen, wsTempFolder );
	}

	if ( !pFontFile ) 
	{
		MemUtilsFree( pCodeToGID );
	}
	
	return pFontFile;
}

SFont *SFontEngine::GetFont(SFontFile *pFontFile, double *pTextMatrix, double *pCTM) 
{
	double arrMatrix[4];

	arrMatrix[0] =    pTextMatrix[0] * pCTM[0] + pTextMatrix[1] * pCTM[2];
	arrMatrix[1] = -( pTextMatrix[0] * pCTM[1] + pTextMatrix[1] * pCTM[3]);
	arrMatrix[2] =    pTextMatrix[2] * pCTM[0] + pTextMatrix[3] * pCTM[2];
	arrMatrix[3] = -( pTextMatrix[2] * pCTM[1] + pTextMatrix[3] * pCTM[3]);

	if ( fabs( arrMatrix[0] * arrMatrix[3] - arrMatrix[1] * arrMatrix[2] ) < 0.01 ) 
	{
		// Чтобы избежать вырожденных матриц
		arrMatrix[0] = 0.01;  arrMatrix[1] = 0;
		arrMatrix[2] = 0;     arrMatrix[3] = 0.01;
	}

	SFont *pFont = m_pFontCache[0];
	if ( pFont && pFont->Matches( pFontFile, arrMatrix, pTextMatrix ) ) 
	{
		return pFont;
	}

	for ( int nFontIndex = 1; nFontIndex < sfontCacheSize; ++nFontIndex ) 
	{
		pFont = m_pFontCache[nFontIndex];
		if ( pFont && pFont->Matches( pFontFile, arrMatrix, pTextMatrix ) ) 
		{
			for ( int nJ = nFontIndex; nJ > 0; --nJ ) 
			{
				m_pFontCache[nJ] = m_pFontCache[nJ - 1];
			}
			m_pFontCache[0] = pFont;
			return pFont;
		}
	}

	pFont = pFontFile->MakeFont( arrMatrix, pTextMatrix );
	if ( m_pFontCache[sfontCacheSize - 1] ) 
	{
		delete m_pFontCache[sfontCacheSize - 1];
	}
	for ( int nJ = sfontCacheSize - 1; nJ > 0; --nJ )
	{
		m_pFontCache[nJ] = m_pFontCache[nJ - 1];
	}
	m_pFontCache[0] = pFont;
	return pFont;
}
