// JBig2File.cpp : Implementation of CJBig2File


#include "JBig2File.h"

#include "Encoder/jbig2encoder.h"

#if defined(_WIN32) || defined(_WIN64)
	#include <tchar.h>
#endif

#include "../../../common/File.h"

CJBig2File::CJBig2File()
{
	m_bDuplicateLineRemoval = false;
	m_bPDFMode				= true;
	m_bSymbolMode			= false;
	m_bRefine				= false;
	m_bUpscale2x			= false;
	m_bUpscale4x			= false;
	m_bSegment				= false;
	
	m_dTreshold				= 0.85;	
	m_nBwTreshold			= 188;
}

bool CJBig2File::MemoryToJBig2(unsigned char* pBufferBGRA ,int BufferSize, int nWidth, int nHeight, std::wstring sDstFileName, bool isBGRA)
{
	// check for valid input parameters

///////////////////////////////////////////////////////////
	if ( NULL == pBufferBGRA )	return false;

	int lBufferSize   = BufferSize;
	unsigned char *pSourceBuffer = pBufferBGRA;

	PIX  *pSource = pixCreate( nWidth, nHeight, 32 );
	if ( !pSource )	return false;

    unsigned char ShiftR = isBGRA ? 2 : 0;
    unsigned char ShiftG = 1;
    unsigned char ShiftB = isBGRA ? 0 : 2;
	for ( int nY = 0; nY < nHeight; nY++ )
	{
		for ( int nX = 0; nX < nWidth; nX++, pSourceBuffer += 3 )//todooo сделать 3 ? 4
		{
            pixSetRGBPixel( pSource, nX, nY, pSourceBuffer[ ShiftR ], pSourceBuffer[ ShiftG ], pSourceBuffer[ ShiftB ] );
		}
	}


	jbig2ctx *pContext = jbig2_init( m_dTreshold, 0.5, 0, 0, ! m_bPDFMode,  m_bRefine ? 10 : -1 );

	// Пока сделаем запись одной картинки в JBig2
	// TO DO: надо будет сделать запись нескольких картинок в 1 JBig2 файл

	// Убираем ColorMap
	PIX *pPixL = NULL;
	if ( NULL == ( pPixL = pixRemoveColormap( pSource, REMOVE_CMAP_BASED_ON_SRC ) ) ) 
	{
		pixDestroy( &pSource );
		jbig2_destroy( pContext );
		return false;
	}
	pixDestroy( &pSource );

	PIX *pPixT = NULL;
	if ( pPixL->d > 1 ) 
	{
		PIX *pGray = NULL;

		if ( pPixL->d > 8 ) 
		{
			pGray = pixConvertRGBToGrayFast( pPixL );
			if ( !pGray )
			{
				pixDestroy( &pSource );
				jbig2_destroy( pContext );
				return false;
			}
		} 
		else 
		{
			pGray = pixClone( pPixL );
		}

		if (  m_bUpscale2x ) 
		{
			pPixT = pixScaleGray2xLIThresh( pGray,  m_nBwTreshold );
		} 
		else if (  m_bUpscale4x ) 
		{
			pPixT = pixScaleGray4xLIThresh( pGray,  m_nBwTreshold );
		} 
		else 
		{
			pPixT = pixThresholdToBinary( pGray,  m_nBwTreshold );
		}

		pixDestroy( &pGray );
	} 
	else 
	{
		pPixT = pixClone( pPixL );
	}

	if ( m_sOutputTreshold.length() > 0 ) 
	{
		pixWrite( m_sOutputTreshold.c_str(), pPixT, IFF_BMP );
	}

	if (  m_bSegment && pPixL->d > 1 ) 
	{
		PIX *pGraphics = segment_image( pPixT, pPixL );
		if ( pGraphics ) 
		{
			char *sFilename;
			asprintf( &sFilename, "%s.%04d.%s", m_sBaseName.c_str(), 0, ".bmp" );
			pixWrite( sFilename, pGraphics, IFF_BMP );
			free( sFilename );
		} 
		if ( !pPixT ) 
		{
			// Ничего не делаем
			return true;
		}
	}

	pixDestroy( &pPixL );

	if ( !m_bSymbolMode ) 
	{
		int nLength = 0;
		uint8_t *pBuffer = jbig2_encode_generic( pPixT, !m_bPDFMode, 0, 0, m_bDuplicateLineRemoval, &nLength );

		bool bRes = true;
        NSFile::CFileBinary file;
        if (file.CreateFileW(sDstFileName ) == true )
        {
            file.WriteFile(pBuffer, nLength);
            file.CloseFile();
			bRes = true;
        }
		else
			bRes = false;

		pixDestroy( &pPixT );
		if ( pBuffer ) free( pBuffer );
		jbig2_destroy( pContext );

		return bRes;
	}

	int nNumPages = 1;
	jbig2_add_page( pContext, pPixT );
	pixDestroy( &pPixT );

	int nLength = 0;
	uint8_t *pBuffer = jbig2_pages_complete( pContext, &nLength );
	if ( !pBuffer )
	{
		jbig2_destroy( pContext );
		return false;
	}

	if ( m_bPDFMode ) 
	{
		std::wstring sFileName = sDstFileName;//m_sBaseName + _T(".sym");

        NSFile::CFileBinary file;
        if ( file.CreateFileW(sFileName) == false)
		{
			free( pBuffer );
			jbig2_destroy( pContext );
			return false;
		}
        file.WriteFile( pBuffer, nLength );
        file.CloseFile();
	}
	free( pBuffer );

	for ( int nIndex = 0; nIndex < nNumPages; ++nIndex ) 
	{
		pBuffer = jbig2_produce_page( pContext, nIndex, -1, -1, &nLength );
		if ( m_bPDFMode ) 
		{
            std::wstring sFileName = m_sBaseName + L".0000";

            NSFile::CFileBinary file;
            if ( file.CreateFileW(sFileName) ==false)
            {
				free( pBuffer );
				jbig2_destroy( pContext );
				return false;
			}
            file.WriteFile( pBuffer, nLength );
            file.CloseFile();
		} 
		free( pBuffer );
	}

	jbig2_destroy( pContext );

	return true;
}


// CJBig2File

