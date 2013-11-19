#include "stdafx.h"

#include <stdio.h>
#include "MemoryUtils.h"
#include "StringExt.h"
#include "File.h"
#include "FontFileTrueType.h"
#include "FontFileType1C.h"
#include "SGlyphLibFontFile.h"
#include "SGlyphLibFontEngine.h"

//-------------------------------------------------------------------------------------------------------------------------------

static void FileWrite(void *pStream, char *sData, int nLen) 
{
	::fwrite( sData, 1, nLen, (FILE *)pStream );
}

//-------------------------------------------------------------------------------------------------------------------------------
// SGlyphLibFontEngine
//-------------------------------------------------------------------------------------------------------------------------------

SGlyphLibFontEngine::SGlyphLibFontEngine(BOOL bAntiAliasing, FT_Library oLibary) 
{
	FT_Int nMajor, nMinor, nPatch;

	m_bAntiAliasing = bAntiAliasing;
	m_oLibrary      = oLibary;

	// учитываем что до версии FT 2.1.8, CID шрифты были нумерованы по CID вместо GID
	FT_Library_Version( m_oLibrary, &nMajor, &nMinor, &nPatch );
	m_bUseCIDs = nMajor > 2 || ( nMajor == 2 && ( nMinor > 1 || ( nMinor == 1 && nPatch > 7 ) ) );
}

SGlyphLibFontEngine *SGlyphLibFontEngine::Init(BOOL bAntiAliasing) 
{
	FT_Library pLibrary = NULL;

	if ( FT_Init_FreeType( &pLibrary ) ) 
	{
		return NULL;
	}
	return new SGlyphLibFontEngine( bAntiAliasing, pLibrary );
}

SGlyphLibFontEngine::~SGlyphLibFontEngine() 
{
	FT_Done_FreeType( m_oLibrary );
}

SFontFile *SGlyphLibFontEngine::LoadType1Font(SFontFileID *pFontFileID, wchar_t *wsFileName, BOOL bDeleteFile, char **ppEncoding) 
{
	return SGlyphLibFontFile::LoadType1Font( this, pFontFileID, wsFileName, bDeleteFile, ppEncoding );
}

SFontFile *SGlyphLibFontEngine::LoadType1CFont(SFontFileID *pFontFileID, wchar_t *wsFileName, BOOL bDeleteFile, char **ppEncoding) 
{
	return SGlyphLibFontFile::LoadType1Font( this, pFontFileID, wsFileName, bDeleteFile, ppEncoding );
}

SFontFile *SGlyphLibFontEngine::LoadOpenTypeT1CFont(SFontFileID *pFontFileID, wchar_t *wsFileName, BOOL bDeleteFile, char **ppEncoding) 
{
	return SGlyphLibFontFile::LoadType1Font( this, pFontFileID, wsFileName, bDeleteFile, ppEncoding );
}

SFontFile *SGlyphLibFontEngine::LoadCIDFont(SFontFileID *pFontFileID, wchar_t *wsFileName, BOOL bDeleteFile) 
{
	unsigned short *pCIDToGIDMap;
	int nCIDsCount = 0;
	CFontFileType1C *pT1CFontFile;
	if ( m_bUseCIDs ) 
	{
		pCIDToGIDMap = NULL;
		nCIDsCount = 0;
	} 
	else if ( ( pT1CFontFile = CFontFileType1C::LoadFromFile( wsFileName ) ) ) 
	{
		pCIDToGIDMap = pT1CFontFile->GetCIDToGIDMap( &nCIDsCount );
		delete pT1CFontFile;
	} 
	else 
	{
		pCIDToGIDMap = NULL;
		nCIDsCount = 0;
	}

	SFontFile *pFontFile = SGlyphLibFontFile::LoadCIDFont( this, pFontFileID, wsFileName, bDeleteFile, pCIDToGIDMap, nCIDsCount );
	if ( !pFontFile ) 
	{
		MemUtilsFree( pCIDToGIDMap );
	}
	return pFontFile;
}

SFontFile *SGlyphLibFontEngine::LoadOpenTypeCFFFont(SFontFileID *pFontFileID, wchar_t *wsFileName, BOOL bDeleteFile) 
{
	unsigned short *pCIDToGIDMap = NULL;
	int nCIDsCount = 0;
	CFontFileTrueType *pTTFontFile;
	if ( !m_bUseCIDs ) 
	{
		if ( ( pTTFontFile = CFontFileTrueType::LoadFromFile( wsFileName ) ) ) 
		{
			if ( pTTFontFile->IsOpenTypeCFF() ) 
			{
				pCIDToGIDMap = pTTFontFile->GetCIDToGIDMap( &nCIDsCount );
			}
			delete pTTFontFile;
		}
	}

	SFontFile *pFontFile = SGlyphLibFontFile::LoadCIDFont( this, pFontFileID, wsFileName, bDeleteFile, pCIDToGIDMap, nCIDsCount );
	if ( !pFontFile ) 
	{
		MemUtilsFree( pCIDToGIDMap );
	}
	return pFontFile;
}

SFontFile *SGlyphLibFontEngine::LoadTrueTypeFont(SFontFileID *pFontFileID, wchar_t *wsFileName, BOOL bDeleteFile, unsigned short *pCodeToGID, int nCodeToGIDLen, wchar_t *wsTempFolder) 
{
	CFontFileTrueType *pTTFontFile;
	if ( !( pTTFontFile = CFontFileTrueType::LoadFromFile( wsFileName ) ) ) 
	{
		return NULL;
	}

	CStringW wsTempFileName = _T("");
	FILE *pTempFile = NULL;
	if ( !OpenTempFile( &wsTempFileName, &pTempFile, _T("wb"), NULL, wsTempFolder ) ) 
	{
		delete pTTFontFile;
		return NULL;
	}
	pTTFontFile->WriteTTF( &FileWrite, pTempFile );
	delete pTTFontFile;
	fclose( pTempFile );

	SFontFile *pFontFile = SGlyphLibFontFile::LoadTrueTypeFont( this, pFontFileID, wsTempFileName.GetBuffer(), TRUE, pCodeToGID, nCodeToGIDLen );
	if ( pFontFile ) 
	{
		if ( bDeleteFile ) 
		{
			_wunlink( wsFileName );
		}
	} 
	else 
	{
		_wunlink( wsTempFileName.GetBuffer() );
	}

	return pFontFile;
}
