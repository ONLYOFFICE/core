#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>
#include "MemoryUtils.h"
#include "Constants.h"
#include "Error.h"
#include "GState.h"
#include "Object.h"
#include "Stream.h"
#include "ExtractImageOutputDev.h"

#pragma pack(push, 1)

struct ColorTableEntry
{
	BYTE nRed;
	BYTE nGreen;
	BYTE nBlue;
	BYTE nReserved;
};

#pragma pack(pop)

//-------------------------------------------------------------------------------------------------------------------------------
// ExtractImageOutputDev
//-------------------------------------------------------------------------------------------------------------------------------


ExtractImageOutputDev::ExtractImageOutputDev(GlobalParams *pGlobalParams, char *sFilePrefix, BOOL bDumpJPEG, BOOL bCountImages) 
{
	m_pGlobalParams = pGlobalParams;
	if ( sFilePrefix )
		m_sFilePrefix = CopyString( sFilePrefix );
	else 
		m_sFilePrefix = NULL;

	if ( m_sFilePrefix )
		m_sFileName = (char *)MemUtilsMalloc( strlen( m_sFilePrefix ) + 20 );
	else
		m_sFileName = NULL;

	m_bDumpJPEG     = bDumpJPEG;
	m_nImageCount   = 0;
	m_bValid        = TRUE;

	m_bCountImages  = bCountImages;
}

ExtractImageOutputDev::~ExtractImageOutputDev() 
{
	MemUtilsFree( m_sFileName );
	MemUtilsFree( m_sFilePrefix );
}

void ExtractImageOutputDev::DrawImageMask(GrState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, BOOL bInvert, BOOL bInlineImage) 
{
	FILE *pFile = NULL;

	// JPEG
	if ( m_bDumpJPEG && pStream->GetType() == strDCT && !bInlineImage ) 
	{
		++m_nImageCount;
		if ( m_bCountImages || !m_sFileName || !m_sFilePrefix )
			return;

		sprintf( m_sFileName, "%s\\%04d.jpg", m_sFilePrefix, m_nImageCount);
		if ( !( pFile = fopen( m_sFileName, "wb" ) ) ) 
		{
			// TO DO: Error "Couldn't open image file"
			return;
		}

		pStream = ((DCTStream *)pStream)->GetRawStream();
		pStream->Reset();

		// Копируем поток в файл
		int nChar = 0;
		while ( ( nChar = pStream->GetChar() ) != EOF )
			fputc( nChar, pFile );

		pStream->Close();
		fclose( pFile );
	} 
	else 
	{
		int nLeftBytes = 0;
		while ( div_t(div( ( ( nWidth + 7 ) / 8 ), 4 )).rem != 0 )
		{
			nWidth -= 8;
			nLeftBytes++;
			if ( nWidth <= 0 )
			{
				// TO DO: Error "To small picture"
				return;
			}
		}

		++m_nImageCount;
		if ( m_bCountImages || !m_sFileName || !m_sFilePrefix )
			return;

		sprintf( m_sFileName, "%s\\%04d.bmp", m_sFilePrefix, m_nImageCount);

		if ( !( pFile = ::fopen( m_sFileName, "wb" ) ) )
		{
			// TO DO: Error "Couldn't open image file"
			return;
		}


		BITMAPFILEHEADER oFileHeader;
		oFileHeader.bfType      = 'M' * 256 + 'B';
		oFileHeader.bfSize      = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 2 * sizeof(ColorTableEntry) + nHeight * ((nWidth + 7) / 8);
		oFileHeader.bfReserved1 = 0;
		oFileHeader.bfReserved2 = 0;
		oFileHeader.bfOffBits   = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 2 * sizeof(ColorTableEntry);


		BITMAPINFOHEADER oInfoHeader;
		oInfoHeader.biSize          = sizeof(BITMAPINFOHEADER);
		oInfoHeader.biWidth         = nWidth;
		oInfoHeader.biHeight        = nHeight;
		oInfoHeader.biPlanes        = 1;
		oInfoHeader.biBitCount      = 1;
		oInfoHeader.biCompression   = BI_RGB;
		oInfoHeader.biSizeImage     = nHeight * ((nWidth + 7) / 8);
		oInfoHeader.biXPelsPerMeter = 0;
		oInfoHeader.biYPelsPerMeter = 0;
		oInfoHeader.biClrUsed       = 256;
		oInfoHeader.biClrImportant  = 0;

		ColorTableEntry oFirstColor, oSecondColor;
		oFirstColor.nRed      = 255;
		oFirstColor.nGreen    = 255;
		oFirstColor.nBlue     = 255;
		oFirstColor.nReserved = 0;

		oSecondColor.nRed      = 0;
		oSecondColor.nGreen    = 0;
		oSecondColor.nBlue     = 0;
		oSecondColor.nReserved = 0;

		::fwrite( &oFileHeader,  1, sizeof(BITMAPFILEHEADER), pFile );
		::fwrite( &oInfoHeader,  1, sizeof(BITMAPINFOHEADER), pFile );
		::fwrite( &oFirstColor,  1, sizeof(ColorTableEntry),  pFile );
		::fwrite( &oSecondColor, 1, sizeof(ColorTableEntry),  pFile );

		long lPos = ::ftell( pFile );

		pStream->Reset();

		// Копируем поток в файл
		int nSize = nHeight * ((nWidth + 7) / 8);

		BYTE *pBuffer = new BYTE[nSize];
		if ( !pBuffer )
		{
			// TO DO: Error "Not enough memory"
			return;
		}
		BYTE *pBufPointer = pBuffer;
		for ( int nY = 0; nY < nHeight; nY++ )
		{
			for ( int nX = 0; nX < (nWidth + 7) / 8; nX++, pBufPointer++ )
			{
				*pBufPointer = pStream->GetChar() ^ 0xff;
			}
			for ( int nIndex = 0; nIndex < nLeftBytes; nIndex ++)
			{
				pStream->GetChar();
			}
		}
		pBufPointer = pBuffer;
		int nStride = ((nWidth + 7) / 8);
		pBufPointer += (nHeight - 1) * nStride;
		nStride = -nStride;

		for( int nRow = nHeight; nRow > 0 ; --nRow, pBufPointer += nStride )
		{
			::fwrite( pBufPointer, 1, abs( nStride ), pFile );
		}
		pStream->Close();
		::fclose( pFile );
		delete []pBuffer;
	}
}

void ExtractImageOutputDev::DrawImage(GrState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GrImageColorMap *pColorMap, int *pnMaskColors, BOOL bInlineImage) 
{
	FILE *pFile = NULL;
	// JPEG
	if ( m_bDumpJPEG && pStream->GetType() == strDCT && ( pColorMap->GetComponentsCount() == 1 || pColorMap->GetComponentsCount() == 3 ) && !bInlineImage ) 
	{
		++m_nImageCount;
		if ( m_bCountImages || !m_sFileName || !m_sFilePrefix  )
			return;
		sprintf( m_sFileName, "%s\\%04d.jpg", m_sFilePrefix, m_nImageCount );

		if ( !( pFile = fopen( m_sFileName, "wb" ) ) ) 
		{
			// TO DO: Error "Couldn't open image file"
			return;
		}

		pStream = ((DCTStream *)pStream)->GetRawStream();
		pStream->Reset();

		// Копируем поток в файл
		int nChar = 0;
		while ( ( nChar = pStream->GetChar() ) != EOF )
			fputc( nChar, pFile );

		pStream->Close();
		fclose( pFile );
	} 
	else if ( pColorMap->GetComponentsCount() == 1 && pColorMap->GetBitsPerComponent() == 1 ) 
	{
		int nLeftBytes = 0;
		while ( div_t(div( ( ( nWidth + 7 ) / 8 ), 4 )).rem != 0 )
		{
			nWidth -= 8;
			nLeftBytes++;
			if ( nWidth <= 0 )
			{
				// TO DO: Error "To small picture"
				return;
			}
		}

		++m_nImageCount;
		if ( m_bCountImages || !m_sFileName || !m_sFilePrefix  )
			return;

		sprintf( m_sFileName, "%s\\%04d.bmp", m_sFilePrefix, m_nImageCount);

		if ( !( pFile = ::fopen( m_sFileName, "wb" ) ) )
		{
			// TO DO: Error "Couldn't open image file"
			return;
		}

		BITMAPFILEHEADER oFileHeader;
		oFileHeader.bfType      = 'M' * 256 + 'B';
		oFileHeader.bfSize      = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 2 * sizeof(ColorTableEntry) + nHeight * ((nWidth + 7) / 8);
		oFileHeader.bfReserved1 = 0;
		oFileHeader.bfReserved2 = 0;
		oFileHeader.bfOffBits   = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 2 * sizeof(ColorTableEntry);


		BITMAPINFOHEADER oInfoHeader;
		oInfoHeader.biSize          = sizeof(BITMAPINFOHEADER);
		oInfoHeader.biWidth         = nWidth;
		oInfoHeader.biHeight        = nHeight;
		oInfoHeader.biPlanes        = 1;
		oInfoHeader.biBitCount      = 1;
		oInfoHeader.biCompression   = BI_RGB;
		oInfoHeader.biSizeImage     = nHeight * ((nWidth + 7) / 8);
		oInfoHeader.biXPelsPerMeter = 0;
		oInfoHeader.biYPelsPerMeter = 0;
		oInfoHeader.biClrUsed       = 0;
		oInfoHeader.biClrImportant  = 0;

		ColorTableEntry oFirstColor, oSecondColor;
		oFirstColor.nRed      = 255;
		oFirstColor.nGreen    = 255;
		oFirstColor.nBlue     = 255;
		oFirstColor.nReserved = 0;

		oSecondColor.nRed      = 0;
		oSecondColor.nGreen    = 0;
		oSecondColor.nBlue     = 0;
		oSecondColor.nReserved = 0;

		::fwrite( &oFileHeader,  1, sizeof(BITMAPFILEHEADER), pFile );
		::fwrite( &oInfoHeader,  1, sizeof(BITMAPINFOHEADER), pFile );
		::fwrite( &oFirstColor,  1, sizeof(ColorTableEntry),  pFile );
		::fwrite( &oSecondColor, 1, sizeof(ColorTableEntry),  pFile );

		pStream->Reset();

		// Копируем поток в файл
		int nSize = nHeight * ((nWidth + 7) / 8);

		BYTE *pBuffer = new BYTE[nSize];
		if ( !pBuffer )
		{
			// TO DO: Error "Not enough memory"
			return;
		}
		BYTE *pBufPointer = pBuffer;
		for ( int nY = 0; nY < nHeight; nY++ )
		{
			for ( int nX = 0; nX < (nWidth + 7) / 8; nX++, pBufPointer++ )
			{
				*pBufPointer = pStream->GetChar() ^ 0xff;
			}
			for ( int nIndex = 0; nIndex < nLeftBytes; nIndex ++)
			{
				pStream->GetChar();
			}
		}
		pBufPointer = pBuffer;
		int nStride = ((nWidth + 7) / 8);
		pBufPointer += (nHeight - 1) * nStride;
		nStride = -nStride;

		for( int nRow = nHeight; nRow > 0 ; --nRow, pBufPointer += nStride )
		{
			::fwrite( pBufPointer, 1, -nStride, pFile );
		}
		pStream->Close();
		::fclose( pFile );
		delete []pBuffer;
	} 
	else 
	{
		++m_nImageCount;
		if ( m_bCountImages || !m_sFileName || !m_sFilePrefix  )
			return;

		sprintf( m_sFileName, "%s\\%04d.bmp", m_sFilePrefix, m_nImageCount);

		if ( !( pFile = ::fopen( m_sFileName, "wb" ) ) ) 
		{
			// TO DO: Error "Couldn't open image file"
			return;
		}

		BITMAPFILEHEADER oFileHeader;
		oFileHeader.bfType      = 'M' * 256 + 'B';
		oFileHeader.bfSize      = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPCOREHEADER) + nWidth * nHeight * 4;
		oFileHeader.bfReserved1 = 0;
		oFileHeader.bfReserved2 = 0;
		oFileHeader.bfOffBits   = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPCOREHEADER);

		BITMAPCOREHEADER oCoreHeader;
		oCoreHeader.bcSize     = sizeof(BITMAPCOREHEADER);
		oCoreHeader.bcWidth    = nWidth;
		oCoreHeader.bcHeight   = nHeight;
		oCoreHeader.bcPlanes   = 1;
		oCoreHeader.bcBitCount = 32;

		::fwrite( &oFileHeader, 1, sizeof(BITMAPFILEHEADER), pFile );
		::fwrite( &oCoreHeader, 1, sizeof(BITMAPCOREHEADER), pFile );

		ImageStream *pImageStream = new ImageStream( pStream, nWidth, pColorMap->GetComponentsCount(), pColorMap->GetBitsPerComponent() );
		if ( !pImageStream )
		{
			// TO DO: Error "Not enough memory"
			return;
		}

		pImageStream->Reset();

		BYTE *pBuffer = new BYTE[ nWidth * nHeight * 4 ];
		if ( !pBuffer )
		{
			// TO DO: Error "Not enough memory"
			delete pImageStream;
			return;
		}

		BYTE *pBufPointer = pBuffer;
		for ( int nY = 0; nY < nHeight; nY++ )
		{
			unsigned char *pImageLine = pImageStream->GetNextLine();
			for ( int nX = 0; nX < nWidth; nX++, pBufPointer += 4 )
			{
				GrRGB oRGB;
				pColorMap->GetRGB( pImageLine, &oRGB );

				pBufPointer[0] = ColorToByte(oRGB.b);
				pBufPointer[1] = ColorToByte(oRGB.g);
				pBufPointer[2] = ColorToByte(oRGB.r);
				pBufPointer[3] = 255;

				pImageLine += pColorMap->GetComponentsCount();
			}
		}
		delete pImageStream;

		pBufPointer = pBuffer;
		int nStride = nWidth * 4;
		pBufPointer += (nHeight - 1) * nStride;
		nStride = -nStride;

		for( int nRow = nHeight; nRow > 0 ; --nRow, pBufPointer += nStride )
		{
			::fwrite( pBufPointer, 1, -nStride, pFile );
		}

		::fclose( pFile );
		delete []pBuffer;
	}
}
