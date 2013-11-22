#include "stdafx.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "FontEngine.h"
#include "FontFile.h"
#include "Font.h"

#include "FT_FontEngine.h"
#include "List.h"


//-------------------------------------------------------------------------------------------------------------------------------
// CFontEngine
//-------------------------------------------------------------------------------------------------------------------------------

CFontEngine::CFontEngine(BOOL bAntiAliasing, BOOL bUseKerning, BOOL bLoadWinFonts, CString sWinFontsXml, CString wsFontDir) 
{
	for ( int nIndex = 0; nIndex < MAX_FONT_CACHE_SIZE; ++nIndex ) 
	{
		m_pFontCache[nIndex] = NULL;
	}

	// Инициализируем библиотеку FreeType
	m_pFreeTypeEngine = CFreeTypeFontEngine::Initialize( bAntiAliasing, bUseKerning );

	// Составляем список установленных шрифтов
	m_pWinFontList = NULL;

	if ( _T("") != sWinFontsXml )
		m_pWinFontList = new CWinFontList( sWinFontsXml );
	else if ( bLoadWinFonts )
	{
		if ( _T("") != wsFontDir )
			m_pWinFontList = new CWinFontList( m_pFreeTypeEngine->m_pLibrary, wsFontDir );
		else
			m_pWinFontList = new CWinFontList( m_pFreeTypeEngine->m_pLibrary );
	}

	m_pDefaultFont[0] = NULL;
	m_pDefaultFont[1] = NULL;
	m_pDefaultFont[2] = NULL;
	m_pDefaultFont[3] = NULL;

	for ( int nIndex = 0; nIndex < INSTALLED_FONTS_CACHE_SIZE; nIndex++ )
	{
		m_arrInstalledFontsCache[nIndex].wsFamilyName = _T("");
		m_arrInstalledFontsCache[nIndex].lStyle       = 0;
		m_arrInstalledFontsCache[nIndex].pWinFontInfo = NULL;
	}
}

CFontEngine::CFontEngine(BOOL bAntiAliasing, BOOL bUseKerning, BOOL bLoadWinFonts, BYTE* pBuffer, CString wsFontDir) 
{
	for ( int nIndex = 0; nIndex < MAX_FONT_CACHE_SIZE; ++nIndex ) 
	{
		m_pFontCache[nIndex] = NULL;
	}

	// Инициализируем библиотеку FreeType
	m_pFreeTypeEngine = CFreeTypeFontEngine::Initialize( bAntiAliasing, bUseKerning );

	// Составляем список установленных шрифтов
	m_pWinFontList = NULL;

	if ( NULL != pBuffer )
		m_pWinFontList = new CWinFontList( pBuffer, wsFontDir );
	else if ( bLoadWinFonts )
	{
		if ( _T("") != wsFontDir )
			m_pWinFontList = new CWinFontList( m_pFreeTypeEngine->m_pLibrary, wsFontDir );
		else
			m_pWinFontList = new CWinFontList( m_pFreeTypeEngine->m_pLibrary );
	}

	m_pDefaultFont[0] = NULL;
	m_pDefaultFont[1] = NULL;
	m_pDefaultFont[2] = NULL;
	m_pDefaultFont[3] = NULL;

	for ( int nIndex = 0; nIndex < INSTALLED_FONTS_CACHE_SIZE; nIndex++ )
	{
		m_arrInstalledFontsCache[nIndex].wsFamilyName = _T("");
		m_arrInstalledFontsCache[nIndex].lStyle       = 0;
		m_arrInstalledFontsCache[nIndex].pWinFontInfo = NULL;
	}
}

CFontEngine::~CFontEngine() 
{
	for ( int nIndex = 0; nIndex < MAX_FONT_CACHE_SIZE; ++nIndex ) 
	{
		if ( m_pFontCache[nIndex] ) 
		{
			delete m_pFontCache[nIndex];
		}
	}

	for ( int nIndex = 0; nIndex < 4; nIndex++ )
	{
		if ( m_pDefaultFont[nIndex] )
			delete m_pDefaultFont[nIndex];
	}

	if ( m_pFreeTypeEngine ) 
	{
		delete m_pFreeTypeEngine;
	}

	if ( m_pWinFontList )
		delete m_pWinFontList;
}

CFontFile *CFontEngine::GetFontFile(wchar_t *wsFamilyName, BOOL bBold, BOOL bItalic)
{
	long lIndex = 0;
	long lStyle = ( bBold ? 1 : 0 ) + ( bItalic ? 2 : 0 );
	CWinFontInfo *pFontInfo = GetInstalledFont(wsFamilyName, lStyle, &lIndex );

	if ( !pFontInfo )
		return NULL;

	for ( int nIndex = 0; nIndex < MAX_FONT_CACHE_SIZE; ++nIndex ) 
	{
		if ( m_pFontCache[nIndex] ) 
		{
			CFontFile *pFontFile = m_pFontCache[nIndex]->GetFontFile();

			CStringW oTemp1 = pFontFile->m_wsFileName;
			CStringW oTemp2 = pFontInfo->m_wsFontPath;

			oTemp1.MakeLower();
			oTemp2.MakeLower();

			if ( pFontFile && oTemp1 == oTemp2 && pFontFile->m_lIndex == pFontInfo->m_lIndex ) 
			{
				return pFontFile;
			}
		}
	}
	return NULL;
}

CFontFile *CFontEngine::GetFontFile(wchar_t *wsFilePath, long lIndex)
{
	for ( int nIndex = 0; nIndex < MAX_FONT_CACHE_SIZE; ++nIndex ) 
	{
		if ( m_pFontCache[nIndex] ) 
		{
			CFontFile *pFontFile = m_pFontCache[nIndex]->GetFontFile();

			CStringW oTemp1 = pFontFile->m_wsFileName;
			CStringW oTemp2 = CStringW( wsFilePath );

			oTemp1.MakeLower();
			oTemp2.MakeLower();

			if ( pFontFile && oTemp1 == oTemp2 && pFontFile->m_lIndex == lIndex ) 
			{
				return pFontFile;
			}
		}
	}
	return NULL;
}

CFontFile *CFontEngine::LoadFont(wchar_t *wsFamilyName, BOOL bBold, BOOL bItalic) 
{
	long lIndex = 0;
	long lStyle = ( bBold ? 1 : 0 ) + ( bItalic ? 2 : 0 );
	CWinFontInfo *pFontInfo = GetInstalledFont(wsFamilyName, lStyle, &lIndex );

	if ( !pFontInfo )
		return NULL;

	CFontFile *pFontFile = NULL;

	if ( !pFontFile && m_pFreeTypeEngine ) 
	{
		pFontFile = m_pFreeTypeEngine->LoadFont( pFontInfo->m_wsFontPath.GetBuffer(), pFontInfo->m_lIndex );
	}

	return pFontFile;
}

CFontFile *CFontEngine::LoadFont(wchar_t *wsFilePath, long lIndex) 
{
	CFontFile *pFontFile = NULL;

	if ( !pFontFile && m_pFreeTypeEngine ) 
	{
		pFontFile = m_pFreeTypeEngine->LoadFont( wsFilePath, lIndex );
	}

	return pFontFile;
}

CWinFontInfo *CFontEngine::GetInstalledFont(long lIndex)
{
	if ( !m_pWinFontList )
		return NULL;

	int nCount = m_pWinFontList->GetFonts()->GetLength();

	if ( lIndex < 0 )
		lIndex = 0;
	else if ( lIndex >= nCount )
		lIndex = nCount - 1;

	CWinFontInfo *pFontInfo = m_pWinFontList->GetByIndex( lIndex );

	return pFontInfo;
}
CWinFontInfo *CFontEngine::GetInstalledFont(wchar_t *wsFamilyName, long lStyle, long *plIndex)
{
	int nCacheIndex = FindInCache( wsFamilyName, lStyle ); 
	if ( -1 != nCacheIndex ) 
		return m_arrInstalledFontsCache[nCacheIndex].pWinFontInfo;

	if ( !m_pWinFontList )
		return NULL;

	// Ищем все шрифты с данным именем FamilyName
	CList *pFoundList = new CList();
	CSimpleArray<int> arrIndexList;
	if ( !pFoundList )
		return NULL;

	for ( int nIndex = 0; nIndex < m_pWinFontList->GetFonts()->GetLength(); nIndex++ )
	{
		CWinFontInfo *pFontInfo = m_pWinFontList->GetByIndex( nIndex );
		if ( CStringW( wsFamilyName ) == pFontInfo->m_wsFontName )
		{
			// Мы добавляем только ссылки в список, поэтому при удалении не надо делать DeleteCList()
			pFoundList->Append( pFontInfo );
			arrIndexList.Add( nIndex );
		}
	}

	if ( pFoundList->GetLength() <= 0 )
	{
		delete pFoundList;
		return NULL;
	}

	BOOL bBold   = FALSE;
	BOOL bItalic = FALSE;

	switch (lStyle)
	{
	case FontConstants::FontStyleRegular:    bBold = FALSE; bItalic = FALSE; break;
	case FontConstants::FontStyleItalic:     bBold = FALSE; bItalic = TRUE;  break;
	case FontConstants::FontStyleBold:       bBold = TRUE;  bItalic = FALSE; break;
	case FontConstants::FontStyleBoldItalic: bBold = TRUE;  bItalic = TRUE;  break;
	}

	// Ищем шрифт с данным стилем
	int arrStyles[4] = { -1, -1, -1, -1 }; // { Regular, Bold, Italic, Bold Italic }

	for ( int nIndex = 0; nIndex < pFoundList->GetLength(); nIndex++ )
	{
		CWinFontInfo *pCurFont = (CWinFontInfo *)pFoundList->GetByIndex( nIndex );

		if ( arrStyles[3] < 0 && TRUE  == pCurFont->m_bBold && TRUE  == pCurFont->m_bItalic ) 
			arrStyles[3] = nIndex;
		if ( arrStyles[2] < 0 && FALSE == pCurFont->m_bBold && TRUE  == pCurFont->m_bItalic )
			arrStyles[2] = nIndex;
		if ( arrStyles[1] < 0 && TRUE  == pCurFont->m_bBold && FALSE == pCurFont->m_bItalic )
			arrStyles[1] = nIndex;
		if ( arrStyles[0] < 0 && FALSE == pCurFont->m_bBold && FALSE == pCurFont->m_bItalic )
			arrStyles[0] = nIndex;				   
	}

	CWinFontInfo *pFontInfo = NULL;
	BOOL bNeedBold = bBold, bNeedItalic = bItalic;
	if ( bBold && bItalic && arrStyles[3] >= 0 )
	{
		pFontInfo = (CWinFontInfo *)pFoundList->GetByIndex( arrStyles[3] );
		bNeedBold   = FALSE;
		bNeedItalic = FALSE;
		*plIndex    = arrIndexList.GetData()[arrStyles[3]];
	}
	else if ( bBold && bItalic && arrStyles[3] < 0 && arrStyles[1] >= 0 )
	{
		pFontInfo = (CWinFontInfo *)pFoundList->GetByIndex( arrStyles[1] );
		bNeedBold = FALSE;
		*plIndex  = arrIndexList.GetData()[arrStyles[1]];
	}
	else if ( bBold && bItalic && arrStyles[3] < 0 && arrStyles[2] >= 0 )
	{
		pFontInfo = (CWinFontInfo *)pFoundList->GetByIndex( arrStyles[2] );
		bNeedItalic = FALSE;
		*plIndex    = arrIndexList.GetData()[arrStyles[2]];
	}
	else if ( bBold && bItalic && arrStyles[3] < 0 && arrStyles[0] >= 0 )
	{
		pFontInfo = (CWinFontInfo *)pFoundList->GetByIndex( arrStyles[0] );
		*plIndex  = arrIndexList.GetData()[arrStyles[0]];
	}
	else if ( bBold && !bItalic && arrStyles[1] >= 0 )
	{
		pFontInfo = (CWinFontInfo *)pFoundList->GetByIndex( arrStyles[1] );
		bNeedBold = FALSE;
		*plIndex  = arrIndexList.GetData()[arrStyles[1]];
	}
	else if ( bBold && !bItalic && arrStyles[1] < 0 && arrStyles[0] >= 0 )
	{
		pFontInfo = (CWinFontInfo *)pFoundList->GetByIndex( arrStyles[0] );
		*plIndex  = arrIndexList.GetData()[arrStyles[0]];
	}
	else if ( !bBold && bItalic && arrStyles[2] >= 0 )
	{
		pFontInfo = (CWinFontInfo *)pFoundList->GetByIndex( arrStyles[2] );
		bNeedItalic = FALSE;
		*plIndex    = arrIndexList.GetData()[arrStyles[2]];
	}
	else if ( !bBold && bItalic && arrStyles[2] < 0 && arrStyles[0] >= 0 )
	{
		pFontInfo = (CWinFontInfo *)pFoundList->GetByIndex( arrStyles[0] );
		*plIndex  = arrIndexList.GetData()[arrStyles[0]];
	}
	else if ( !bBold && !bItalic && arrStyles[0] >= 0 )
	{
		pFontInfo = (CWinFontInfo *)pFoundList->GetByIndex( arrStyles[0] );
		*plIndex  = arrIndexList.GetData()[arrStyles[0]];
	}
	else
	{
		// если не один из предыдущих вариантов не прошел, тогда берем первый попавшийся фонт
		// запоминая, нужно ли в будущем делать Bold и Italic
		int nIndex = 0;
		for ( ; nIndex < 4; nIndex++ )
			if ( arrStyles[nIndex] >= 0 )
				break;

		if ( nIndex < 4 )
		{
			switch ( nIndex )
			{
			case 0: break;
			case 1: bNeedBold   = FALSE; break;
			case 2: bNeedItalic = FALSE; break;
			case 3: bNeedItalic = FALSE; bNeedBold = FALSE; break;
			}
			pFontInfo = (CWinFontInfo *)pFoundList->GetByIndex( arrStyles[nIndex] );
			*plIndex  = arrIndexList.GetData()[arrStyles[nIndex]];
		}
	}
	delete pFoundList;

	TInstalledFontCacheEntry oEntry;
	oEntry.lStyle = lStyle;
	oEntry.pWinFontInfo = pFontInfo;
	oEntry.wsFamilyName = CStringW( wsFamilyName );
	AddToCache( oEntry );
	return pFontInfo;
}
CWinFontInfo *CFontEngine::GetInstalledFont(CString sParamsXml)
{
	if ( !m_pWinFontList )
		return NULL;

	return m_pWinFontList->GetByParams( sParamsXml );
}
CWinFontInfo *CFontEngine::GetInstalledFont(wchar_t* wsAltFontName, long lCharset, wchar_t* wsFamilyClass, long lStyle, BOOL bFixed, wchar_t *wsPanose, long lUnicodeRange1, long lUnicodeRange2, long lUnicodeRange3, long lUnicodeRange4, long lCodePageRange1, long lCodePageRange2, long lAvgWidth)
{
	if ( !m_pWinFontList )
		return NULL;

	return m_pWinFontList->GetByParams( wsAltFontName, lCharset, wsFamilyClass, lStyle, bFixed, wsPanose, lUnicodeRange1, lUnicodeRange2, lUnicodeRange3, lUnicodeRange4, lCodePageRange1, lCodePageRange2, lAvgWidth );
}
long CFontEngine::GetInstalledFontsCount()
{
	if ( !m_pWinFontList )
		return 0;

	return m_pWinFontList->GetFonts()->GetLength();
}
BOOL CFontEngine::IsStyleAvailable(wchar_t *wsFamilyName, long lStyle)
{
	if ( !m_pWinFontList )
		return FALSE;

	BOOL bResult = FALSE;

	BOOL bBold   = FALSE;
	BOOL bItalic = FALSE;

	switch (lStyle)
	{
	case FontConstants::FontStyleRegular:    bBold = FALSE; bItalic = FALSE; break;
	case FontConstants::FontStyleItalic:     bBold = FALSE; bItalic = TRUE;  break;
	case FontConstants::FontStyleBold:       bBold = TRUE;  bItalic = FALSE; break;
	case FontConstants::FontStyleBoldItalic: bBold = TRUE;  bItalic = TRUE;  break;
	}

	// Ищем все шрифты с данным именем FamilyName
	CList *pFoundList = new CList();
	if ( !pFoundList )
		return FALSE;

	for ( int nIndex = 0; nIndex < m_pWinFontList->GetFonts()->GetLength(); nIndex++ )
	{
		CWinFontInfo *pFontInfo = m_pWinFontList->GetByIndex( nIndex );
		if ( CStringW( wsFamilyName ) == pFontInfo->m_wsFontName )
		{
			// Мы добавляем только ссылки в список, поэтому при удалении не надо делать DeleteCList()
			pFoundList->Append( pFontInfo );
		}
	}

	// Ищем шрифт с данным стилем
	int arrStyles[4] = { -1, -1, -1, -1 }; // { Regular, Bold, Italic, Bold Italic }

	for ( int nIndex = 0; nIndex < pFoundList->GetLength(); nIndex++ )
	{
		CWinFontInfo *pCurFont = (CWinFontInfo *)pFoundList->GetByIndex( nIndex );

		if ( arrStyles[3] < 0 && TRUE  == pCurFont->m_bBold && TRUE  == pCurFont->m_bItalic ) 
			arrStyles[3] = nIndex;
		if ( arrStyles[2] < 0 && FALSE == pCurFont->m_bBold && TRUE  == pCurFont->m_bItalic )
			arrStyles[2] = nIndex;
		if ( arrStyles[1] < 0 && TRUE  == pCurFont->m_bBold && FALSE == pCurFont->m_bItalic )
			arrStyles[1] = nIndex;
		if ( arrStyles[0] < 0 && FALSE == pCurFont->m_bBold && FALSE == pCurFont->m_bItalic )
			arrStyles[0] = nIndex;				   
	}

	if ( arrStyles[0] >= 0 ) // Если есть Regular, то в крайнем случае остальные из него можем получить с помощью преобразований
		bResult = TRUE;
	else if ( FALSE == bBold && FALSE == bItalic ) // Regular нельзя получить из других стилей
		bResult = FALSE;
	else if ( TRUE == bBold && FALSE == bItalic )
	{
		if ( arrStyles[1] >= 0 )
			bResult = TRUE;
		else
			bResult = FALSE;
	}
	else if ( FALSE == bBold && TRUE == bItalic )
	{
		if ( arrStyles[2] >= 0 )
			bResult = TRUE;
		else
			bResult = FALSE;
	}
	else if ( TRUE == bBold && TRUE == bItalic )
	{
		if ( arrStyles[1] >= 0 || arrStyles[2] >= 0 || arrStyles[3] >= 0 )
			bResult = TRUE;
		else
			bResult = FALSE;
	}

	return bResult;
}
CFont *CFontEngine::GetFont(CFontFile *pFontFile) 
{
	// Если шрифт совпадает с первым в кэше, тогда отдаем указатель на первый шрифт
	CFont *pFont = m_pFontCache[0];
	if ( pFont && pFont->Matches( pFontFile ) ) 
	{
		return pFont;
	}

	// Если шрифт совпадает с не первым шрифтом из кэша, тогда указатель на данный шрифт двигаем в начало кэша и отдаем его
	for ( int nFontIndex = 1; nFontIndex < MAX_FONT_CACHE_SIZE; ++nFontIndex ) 
	{
		pFont = m_pFontCache[nFontIndex];
		if ( pFont && pFont->Matches( pFontFile ) ) 
		{
			for ( int nJ = nFontIndex; nJ > 0; --nJ ) 
			{
				m_pFontCache[nJ] = m_pFontCache[nJ - 1];
			}
			m_pFontCache[0] = pFont;
			return pFont;
		}
	}

	// Если данного шрифта в кэше нет, создаем шрифт и записываем его в начало кэша, а последний шрифт из кэша удаляем(если он там есть)
	pFont = pFontFile->MakeFont();
	if ( m_pFontCache[MAX_FONT_CACHE_SIZE - 1] ) 
	{
		delete m_pFontCache[MAX_FONT_CACHE_SIZE - 1];
	}
	for ( int nJ = MAX_FONT_CACHE_SIZE - 1; nJ > 0; --nJ )
	{
		m_pFontCache[nJ] = m_pFontCache[nJ - 1];
	}
	m_pFontCache[0] = pFont;
	return pFont;
}

void CFontEngine::SetAA(BOOL bAA)
{
	m_pFreeTypeEngine->m_bAntiAliasing = bAA;
}
void CFontEngine::SetDefaultFont(wchar_t *wsFamilyName)
{
	for ( int nIndex = 0; nIndex < 4; nIndex++ )
	{
		BOOL bBold, bItalic;

		switch(  nIndex )
		{
		case 0: bBold = FALSE; bItalic = FALSE; break;
		case 1: bBold = TRUE;  bItalic = FALSE; break;
		case 2: bBold = FALSE; bItalic = TRUE;  break;
		case 3: bBold = TRUE;  bItalic = TRUE;  break;
		}

		long lIndex = 0;
		long lStyle = ( bBold ? 1 : 0 ) + ( bItalic ? 2 : 0 );
		CWinFontInfo *pFontInfo = GetInstalledFont(wsFamilyName, lStyle, &lIndex );

		if ( !pFontInfo )
			return;

		CFontFile *pFontFile = NULL;

		if ( !pFontFile && m_pFreeTypeEngine ) 
		{
			pFontFile = m_pFreeTypeEngine->LoadFont( pFontInfo->m_wsFontPath.GetBuffer(), pFontInfo->m_lIndex );
		}

		if ( !pFontFile )
			return;

		if ( m_pDefaultFont[nIndex] )
			delete m_pDefaultFont[nIndex];

		if ( 0 == nIndex && m_pWinFontList )
			m_pWinFontList->SetDefaultFont( lIndex );

		m_pDefaultFont[nIndex] = pFontFile->MakeFont();
		UpdateStyles( m_pDefaultFont[nIndex], bBold, bItalic );
	}
}
void CFontEngine::SetDefaultFont(wchar_t *wsFilePath, long lIndex)
{
	for ( int nIndex = 0; nIndex < 4; nIndex++ )
	{
		BOOL bBold, bItalic;

		switch(  nIndex )
		{
		case 0: bBold = FALSE; bItalic = FALSE; break;
		case 1: bBold = TRUE;  bItalic = FALSE; break;
		case 2: bBold = FALSE; bItalic = TRUE;  break;
		case 3: bBold = TRUE;  bItalic = TRUE;  break;
		}

		CFontFile *pFontFile = NULL;

		if ( !pFontFile && m_pFreeTypeEngine ) 
		{
			pFontFile = m_pFreeTypeEngine->LoadFont( wsFilePath, lIndex );
		}

		if ( !pFontFile )
			return;

		if ( m_pDefaultFont[nIndex] )
			delete m_pDefaultFont[nIndex];

		m_pDefaultFont[nIndex] = pFontFile->MakeFont();
		UpdateStyles( m_pDefaultFont[nIndex], bBold, bItalic );
	}
}
CFont *CFontEngine::GetDefaultFont(BOOL bBold, BOOL bItalic)
{
	int nIndex = ( bBold ? 1 : 0 ) + ( bItalic ? 2 : 0 );
	return m_pDefaultFont[nIndex];
}
void CFontEngine::AddToCache(TInstalledFontCacheEntry oEntry)
{
	int nLastIndex = 0;
	while ( nLastIndex < INSTALLED_FONTS_CACHE_SIZE && m_arrInstalledFontsCache[nLastIndex].wsFamilyName != _T("") )
		nLastIndex++;

	if ( INSTALLED_FONTS_CACHE_SIZE == nLastIndex )
	{
		for ( int nIndex = INSTALLED_FONTS_CACHE_SIZE - 1; nIndex > 0; nIndex-- )
		{
			m_arrInstalledFontsCache[nIndex] = m_arrInstalledFontsCache[nIndex - 1];
		}

		m_arrInstalledFontsCache[0] = oEntry;
	}
	else
	{
		m_arrInstalledFontsCache[nLastIndex] = oEntry;
	}
}


int  CFontEngine::FindInCache(wchar_t *wsFamilyName, long lStyle)
{
	int nResult = -1;
	for ( int nIndex = 0; nIndex < INSTALLED_FONTS_CACHE_SIZE; nIndex++ )
	{
		if ( _T("") == m_arrInstalledFontsCache[nIndex].wsFamilyName )
			break;

		if ( CStringW( wsFamilyName) == m_arrInstalledFontsCache[nIndex].wsFamilyName && lStyle == m_arrInstalledFontsCache[nIndex].lStyle )
		{
			nResult = nIndex;
			break;
		}
	}

	return nResult;
}