#include "stdafx.h"

#include "FT_FontEngine.h"
#include "FT_FontFile.h"

//-------------------------------------------------------------------------------------------------------------------------------
// CFreeTypeFontEngine
//-------------------------------------------------------------------------------------------------------------------------------

CFreeTypeFontEngine::CFreeTypeFontEngine(BOOL bAntiAliasing, FT_Library pLibary, BOOL bUseKerning) 
{
	FT_Int nMajor, nMinor, nPatch;

	m_bAntiAliasing = bAntiAliasing;
	m_pLibrary      = pLibary;
	m_bUseKerning   = bUseKerning;
	
	// учитываем что до версии FT 2.1.8, CID шрифты были нумерованы по CID вместо GID
	FT_Library_Version( m_pLibrary, &nMajor, &nMinor, &nPatch );

	m_bUseCIDs = nMajor > 2 || ( nMajor == 2 && ( nMinor > 1 || ( nMinor == 1 && nPatch > 7 ) ) );
}

CFreeTypeFontEngine *CFreeTypeFontEngine::Initialize(BOOL bAntiAliasing, BOOL bUseKerning) 
{
	FT_Library pLibrary = NULL;

	if ( FT_Init_FreeType( &pLibrary ) ) 
	{
		return NULL;
	}
	return new CFreeTypeFontEngine( bAntiAliasing, pLibrary, bUseKerning );
}

CFreeTypeFontEngine::~CFreeTypeFontEngine() 
{
	FT_Done_FreeType( m_pLibrary );
}

CFontFile *CFreeTypeFontEngine::LoadFont(wchar_t *wsFilePath, long lIndex) 
{
	CFontFile *pFontFile = CFreeTypeFontFile::LoadFont( this, wsFilePath, lIndex );

	return pFontFile;
}

