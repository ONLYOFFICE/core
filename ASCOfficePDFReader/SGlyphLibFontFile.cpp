#include "stdafx.h"

#include "MemoryUtils.h"
#include "SGlyphLibFontEngine.h"
#include "SGlyphLibFont.h"
#include "SGlyphLibFontFile.h"



//static unsigned long RWRead(FT_Stream stream,unsigned long offset,unsigned char* buffer,unsigned long count) 
//{
//	FILE* pFile = (FILE *)stream->descriptor.pointer;
//	::fseek( pFile, (int)offset, SEEK_SET );
//	return ::fread( (void *)buffer, 1, (int)count, pFile );
//}
//
//static void RWClose(FT_Stream  stream)
//{
//	FILE* pFile = (FILE *)stream->descriptor.pointer;
//	::fclose( pFile );
//}
//
//static FT_Error FT_New_FaceW( FT_Library oLibrary, const wchar_t *wsFilePath, FT_Long lFaceIndex, FT_Face *pFace )
//{
//
//
//	FILE* pFile = _wfopen( wsFilePath, _T("r") );
//	if ( !pFile )
//		return -1; // Error
//
//	::fseek( pFile, 0, SEEK_END );
//	DWORD dwSize = ::ftell( pFile );
//	::fseek( pFile, 0, SEEK_SET );
//
//	BYTE *pBuffer = (BYTE*)malloc( sizeof(BYTE) * dwSize );
//	if ( !pBuffer )
//	{
//		::fclose( pFile );
//		return -1; // Error
//	}
//
//	//::fread( pBuffer, sizeof(unsigned char), dwSize, pFile );
//	//::fclose( pFile );
//
//
//	FT_Open_Args oOpenArgs;
//	memset( (void *)&oOpenArgs, 0, sizeof(oOpenArgs) );
//	//oOpenArgs.flags = FT_OPEN_MEMORY;
//	//oOpenArgs.memory_base = (FT_Bytes)pBuffer;
//	//oOpenArgs.memory_size = (FT_Long)dwSize;
//
//	FT_StreamRec_ oStream;
//	//oStream.base = (unsigned char *)pBuffer;
//	//oStream.size = (unsigned long)dwSize;
//	//oStream.pos = 0;
//
//	oOpenArgs.stream = (FT_Stream) malloc(sizeof *(oOpenArgs.stream));
//	//oOpenArgs.stream = &oStream;
//	oOpenArgs.stream->base  = NULL;
//	oOpenArgs.stream->size  = (unsigned long)dwSize;
//	oOpenArgs.stream->pos   = 0;
//	oOpenArgs.stream->descriptor.pointer = (void *)pFile;
//	oOpenArgs.stream->pathname.pointer   = NULL;
//	oOpenArgs.stream->read  = RWRead;
//	oOpenArgs.stream->close = RWClose;
//	oOpenArgs.num_params    = 0;
//
//	oOpenArgs.flags = FT_OPEN_STREAM;
//
//
//	int nError = 0;
//	if ( nError = FT_Open_Face( oLibrary, (const FT_Open_Args *)&oOpenArgs, lFaceIndex, pFace ) )
//	{
//		if ( nError = FT_New_Memory_Face( oLibrary, (const unsigned char*)pBuffer, (FT_Long)dwSize, lFaceIndex, pFace ) )
//		{
//
//			USES_CONVERSION;
//			if ( nError = FT_New_Face( oLibrary, W2A( wsFilePath ), lFaceIndex, pFace ) )
//			{
//				delete []pBuffer;
//				return -1; // Error
//			}
//		}
//
//	}
//	delete []pBuffer;
//
//	return 0; // Success
//}
//
//-------------------------------------------------------------------------------------------------------------------------------
// SGlyphLibFontFile
//-------------------------------------------------------------------------------------------------------------------------------

SFontFile *SGlyphLibFontFile::LoadType1Font(SGlyphLibFontEngine *pEngine, SFontFileID *pFontFileID, wchar_t *wsFileName, BOOL bDeleteFile, char **ppEncoding) 
{
	USES_CONVERSION;
	FT_Face pFace = NULL;
	if ( FT_New_Face( pEngine->m_oLibrary, W2A(wsFileName), 0, &pFace ) ) 
	{
		return NULL;
	}

	unsigned short *pCodeToGID = (unsigned short *)MemUtilsMallocArray( 256, sizeof(int) );
	for ( int nIndex = 0; nIndex < 256; ++nIndex ) 
	{
		pCodeToGID[nIndex] = 0;

		char *sName = NULL;
		if ( ( sName = ppEncoding[nIndex] ) ) 
		{
			pCodeToGID[nIndex] = (unsigned short)FT_Get_Name_Index( pFace, sName );
		}
	}

	return new SGlyphLibFontFile( pEngine, pFontFileID, wsFileName, bDeleteFile, pFace, pCodeToGID, 256, FALSE );
}

SFontFile *SGlyphLibFontFile::LoadCIDFont(SGlyphLibFontEngine *pEngine, SFontFileID *pFontFileID, wchar_t *wsFileName, BOOL bDeleteFile, unsigned short *pCodeToGID, int nCodeToGIDLen) 
{
	USES_CONVERSION;
	FT_Face oFace;
	if ( FT_New_Face( pEngine->m_oLibrary, W2A(wsFileName), 0, &oFace ) ) 
	{
		return NULL;
	}

	return new SGlyphLibFontFile( pEngine, pFontFileID, wsFileName, bDeleteFile, oFace, pCodeToGID, nCodeToGIDLen, FALSE );
}

SFontFile *SGlyphLibFontFile::LoadTrueTypeFont(SGlyphLibFontEngine *pEngine, SFontFileID *pFontFileID, wchar_t *wsFileName, BOOL bDeleteFile, unsigned short *pCodeToGID, int nCodeToGIDLen) 
{
	USES_CONVERSION;
	FT_Face oFace;
	if ( FT_New_Face( pEngine->m_oLibrary, W2A(wsFileName), 0, &oFace ) ) 
	{
		return NULL;
	}

	return new SGlyphLibFontFile( pEngine, pFontFileID, wsFileName, bDeleteFile, oFace, pCodeToGID, nCodeToGIDLen, TRUE );
}

SGlyphLibFontFile::SGlyphLibFontFile(SGlyphLibFontEngine *pEngine, SFontFileID *pFontFileID, wchar_t *wsFileName, BOOL bDeleteFile, FT_Face oFace, unsigned short *pCodeToGID, int nCodeToGIDLen, BOOL bTrueType):
SFontFile(pFontFileID, wsFileName, bDeleteFile)
{
	m_pEngine       = pEngine;
	m_oFace         = oFace;
	m_pCodeToGID    = pCodeToGID;
	m_nCodeToGIDLen = nCodeToGIDLen;
	m_bTrueType     = bTrueType;
}

SGlyphLibFontFile::~SGlyphLibFontFile() 
{
	if ( m_oFace ) 
	{
		FT_Done_Face( m_oFace );
	}
	if ( m_pCodeToGID ) 
	{
		MemUtilsFree( m_pCodeToGID );
	}
}

SFont *SGlyphLibFontFile::MakeFont(double *pFontMatrix, double *pTextMatrix) 
{
	SFont *pFont = new SGlyphLibFont( this, pFontMatrix, pTextMatrix );
	pFont->InitCache();
	return pFont;
}
