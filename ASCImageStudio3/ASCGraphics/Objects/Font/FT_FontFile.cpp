#include "stdafx.h"

#include "FT_FontEngine.h"
#include "FT_Font.h"
#include "FT_FontFile.h"

//-------------------------------------------------------------------------------------------------------------------------------
// CFreeTypeFontFile
//-------------------------------------------------------------------------------------------------------------------------------

CFontFile *CFreeTypeFontFile::LoadFont(CFreeTypeFontEngine *pEngine, wchar_t *wsFileName, long lIndex) 
{
	USES_CONVERSION;

	FT_Face pFace = NULL;

	// открываем файл
	HANDLE hFile = CreateFile( (LPCWSTR)wsFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
		return NULL; // Невозможно открыть файл

	// мапим этот файл в память - так быстрее читаются данные из файла
	DWORD nFileSize = GetFileSize(hFile, NULL);
	HANDLE hMapFile = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, nFileSize, NULL);
	if (NULL == hMapFile)
	{
		CloseHandle( hFile );
		return NULL; // Невозможно создать отображение файла
	}

	void *pBaseAddress = MapViewOfFile( hMapFile, FILE_MAP_READ, 0, 0, 0 );
	if ( !pBaseAddress )
	{
		CloseHandle( hMapFile );
		CloseHandle( hFile );
		return NULL;
	}

	FT_Open_Args oOpenArgs;
	oOpenArgs.flags = FT_OPEN_MEMORY;
	oOpenArgs.memory_base = (BYTE*)pBaseAddress;
	oOpenArgs.memory_size = (FT_Long)nFileSize;

	if ( FT_Open_Face( pEngine->m_pLibrary, &oOpenArgs, lIndex, &pFace ) )
	//if ( FT_New_Face( pEngine->m_pLibrary, W2A( wsFileName ), lIndex, &pFace ) ) 
	{
		if ( pBaseAddress )
			UnmapViewOfFile( pBaseAddress );
		CloseHandle( hMapFile );
		CloseHandle( hFile );
		return NULL;
	}

	if ( NULL == pFace->charmap && 0 != pFace->num_charmaps )
		FT_Set_Charmap( pFace, pFace->charmaps[0] );

	return new CFreeTypeFontFile( pEngine, wsFileName, lIndex, pFace, hFile, hMapFile, pBaseAddress );
}

CFreeTypeFontFile::CFreeTypeFontFile(CFreeTypeFontEngine *pEngine, wchar_t *wsFileName, long lIndex, FT_Face pFace, HANDLE hFile, HANDLE hMapFile, void *pBaseAddress):
CFontFile(wsFileName, lIndex)
{
	m_pEngine       = pEngine;
	m_pFace         = pFace;

	m_pBaseAddress = pBaseAddress;
	m_hFile        = hFile;
	m_hMapFile     = hMapFile;
}

CFreeTypeFontFile::~CFreeTypeFontFile() 
{
	if ( m_pFace ) 
	{
		FT_Done_Face( m_pFace );
	}

	if ( m_pBaseAddress )
		UnmapViewOfFile( m_pBaseAddress );
	if ( m_hMapFile )
		CloseHandle( m_hMapFile );
	if ( m_hFile )
		CloseHandle( m_hFile );

}

CFont *CFreeTypeFontFile::MakeFont() 
{
	CFont *pFont = new CFreeTypeFont( this );
	pFont->InitCache();
	return pFont;
}
