#include "stdafx.h"

#include <stdio.h>
#include "MemoryUtils.h"
#include "SErrorCodes.h"
#include "SBitmap.h"
#include "File.h"
#include "StringExt.h"

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
// SBitmap
//-------------------------------------------------------------------------------------------------------------------------------

SBitmap::SBitmap(int nWidth, int nHeight, int nRowPad, SColorMode eMode, BOOL bAlpha, BOOL bTopDown) 
{
	m_nWidth  = nWidth;
	m_nHeight = nHeight;
	m_eMode   = eMode;

	switch ( m_eMode ) 
	{
	case colorModeMono1:
		m_nStride = ( m_nWidth + 7 ) >> 3;
		break;
	case colorModeMono8:
		m_nStride = m_nWidth;
		break;
	case colorModeRGB8:
	case colorModeBGR8:
		m_nStride = m_nWidth * 3;
		break;
	}

	m_nStride += nRowPad - 1;
	m_nStride -= m_nStride % nRowPad;

	m_pData = (SColorPointer)MemUtilsMalloc( m_nStride * m_nHeight );

	if ( !bTopDown ) 
	{
		m_pData += ( m_nHeight - 1 ) * m_nStride;
		m_nStride = -m_nStride;
	}

	if ( bAlpha ) 
	{
		m_pAlpha = (unsigned char *)MemUtilsMalloc( m_nWidth * m_nHeight );
	} 
	else 
	{
		m_pAlpha = NULL;
	}
}


SBitmap::~SBitmap() 
{
	if ( m_nStride < 0 ) 
	{
		MemUtilsFree( m_pData + (m_nHeight - 1) * m_nStride );
	} 
	else 
	{
		MemUtilsFree( m_pData );
	}
	MemUtilsFree( m_pAlpha );
}

int SBitmap::WritePNMFile(wchar_t *wsFileName) 
{
	FILE *pFile = NULL;
	SColorPointer pRow = NULL, pCurPixel = NULL;

	if ( !( pFile = ::_wfopen( wsFileName, _T("wb") ) ) ) 
	{
		return SErrorOpenFile;
	}

	switch ( m_eMode ) 
	{
	case colorModeMono1:
		fprintf( pFile, "P4\n%d %d\n", m_nWidth, m_nHeight );
		pRow = m_pData;
		for ( int nY = 0; nY < m_nHeight; ++nY ) 
		{
			pCurPixel = pRow;
			for ( int nX = 0; nX < m_nWidth; nX += 8 ) 
			{
				fputc( *pCurPixel ^ 0xff, pFile );
				++pCurPixel;
			}
			pRow += m_nStride;
		}
		break;

	case colorModeMono8:
		fprintf( pFile, "P5\n%d %d\n255\n", m_nWidth, m_nHeight );
		pRow = m_pData;
		for ( int nY = 0; nY < m_nHeight; ++nY ) 
		{
			pCurPixel = pRow;
			for ( int nX = 0; nX < m_nWidth; ++nX ) 
			{
				fputc( *pCurPixel, pFile );
				++pCurPixel;
			}
			pRow += m_nStride;
		}
		break;

	case colorModeRGB8:
		fprintf( pFile, "P6\n%d %d\n255\n", m_nWidth, m_nHeight );
		pRow = m_pData;
		for ( int nY = 0; nY < m_nHeight; ++nY ) 
		{
			pCurPixel = pRow;
			for ( int nX = 0; nX < m_nWidth; ++nX ) 
			{
				fputc( pCurPixel[0], pFile );
				fputc( pCurPixel[1], pFile );
				fputc( pCurPixel[2], pFile );
				pCurPixel += 3;
			}
			pRow += m_nStride;
		}
		break;

	case colorModeBGR8:
		fprintf( pFile, "P6\n%d %d\n255\n", m_nWidth, m_nHeight );
		pRow = m_pData;
		for ( int nY = 0; nY < m_nHeight; ++nY ) 
		{
			pCurPixel = pRow;
			for ( int nX = 0; nX < m_nWidth; ++nX ) 
			{
				fputc( pCurPixel[2], pFile );
				fputc( pCurPixel[1], pFile );
				fputc( pCurPixel[0], pFile );
				pCurPixel += 3;
			}
			pRow += m_nStride;
		}
		break;
	}

	fclose( pFile );
	return SNoError;
}

int SBitmap::WriteBMPFile(wchar_t *wsFileName) 
{
	FILE *pFile = NULL;
	int nWidth  = m_nWidth;
	int nHeight = m_nHeight;
	SColorPointer pRow = NULL, pCurPixel = NULL;

	if ( !( pFile = ::_wfopen( wsFileName, _T("wb") ) ) ) 
	{
		return SErrorOpenFile;
	}

	int nSize = 0, nStride;
	BYTE *pBuffer = NULL, *pBufPointer;
	BITMAPFILEHEADER oFileHeader;
	BITMAPINFOHEADER oInfoHeader;

	switch ( m_eMode ) 
	{
	case colorModeMono1:

		while ( div_t(div( ( ( nWidth + 7 ) / 8 ), 4 )).rem != 0 )
		{
			nWidth -= 8;
			if ( nWidth <= 0 )
			{
				return SErrorTooSmallBMP;
			}
		}

		oFileHeader.bfType      = 'M' * 256 + 'B';
		oFileHeader.bfSize      = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 2 * sizeof(ColorTableEntry) + nHeight * ((nWidth + 7) / 8);
		oFileHeader.bfReserved1 = 0;
		oFileHeader.bfReserved2 = 0;
		oFileHeader.bfOffBits   = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 2 * sizeof(ColorTableEntry);

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

		nSize = nHeight * ((nWidth + 7) / 8);

		pBuffer = new BYTE[nSize];
		if ( !pBuffer )
		{
			return SErrorMemory; 
		}
		pBufPointer = pBuffer;
		pRow = m_pData;
		for ( int nY = 0; nY < nHeight; nY++ )
		{
			pCurPixel = pRow;
			for ( int nX = 0; nX < (nWidth + 7) / 8; nX++, pBufPointer++ )
			{
				*pBufPointer = *pCurPixel ^ 0xff;
				pCurPixel++;
			}
			pRow += m_nStride;
		}
		pBufPointer = pBuffer;
		nStride = ((nWidth + 7) / 8);
		pBufPointer += (nHeight - 1) * nStride;
		nStride = -nStride;

		for( int nRow = nHeight; nRow > 0 ; --nRow, pBufPointer += nStride )
		{
			::fwrite( pBufPointer, 1, abs( nStride ), pFile );
		}
		delete []pBuffer;
		break;

	case colorModeMono8:

		while ( div_t(div( nWidth, 4 )).rem != 0 )
		{
			nWidth--;
			if ( nWidth <= 0 )
			{
				return SErrorTooSmallBMP;
			}
		}

		oFileHeader.bfType      = 'M' * 256 + 'B';
		oFileHeader.bfSize      = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 256 * sizeof(ColorTableEntry) + nHeight * nWidth;
		oFileHeader.bfReserved1 = 0;
		oFileHeader.bfReserved2 = 0;
		oFileHeader.bfOffBits   = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 256 * sizeof(ColorTableEntry);

		oInfoHeader.biSize          = sizeof(BITMAPINFOHEADER);
		oInfoHeader.biWidth         = nWidth;
		oInfoHeader.biHeight        = nHeight;
		oInfoHeader.biPlanes        = 1;
		oInfoHeader.biBitCount      = 8;
		oInfoHeader.biCompression   = BI_RGB;
		oInfoHeader.biSizeImage     = nHeight * nWidth;
		oInfoHeader.biXPelsPerMeter = 0;
		oInfoHeader.biYPelsPerMeter = 0;
		oInfoHeader.biClrUsed       = 0;
		oInfoHeader.biClrImportant  = 0;

		::fwrite( &oFileHeader,  1, sizeof(BITMAPFILEHEADER), pFile );
		::fwrite( &oInfoHeader,  1, sizeof(BITMAPINFOHEADER), pFile );

		for ( int nIndex = 0; nIndex < 256; nIndex++ )
		{
			ColorTableEntry oColor;
			oColor.nRed      = nIndex;
			oColor.nGreen    = nIndex;
			oColor.nBlue     = nIndex;
			oColor.nReserved = 0;
			::fwrite( &oColor,  1, sizeof(ColorTableEntry), pFile );
		}

		nSize = nHeight * nWidth;

		pBuffer = new BYTE[nSize];
		if ( !pBuffer )
		{
			return SErrorMemory; 
		}
		pBufPointer = pBuffer;
		pRow = m_pData;
		for ( int nY = 0; nY < nHeight; nY++ )
		{
			pCurPixel = pRow;
			for ( int nX = 0; nX < nWidth; nX++, pBufPointer++ )
			{
				*pBufPointer = (BYTE)*pCurPixel;
				pCurPixel++;
			}
			pRow += m_nStride;
		}
		pBufPointer = pBuffer;
		nStride = nWidth;
		pBufPointer += (nHeight - 1) * nStride;
		nStride = -nStride;

		for( int nRow = nHeight; nRow > 0 ; --nRow, pBufPointer += nStride )
		{
			::fwrite( pBufPointer, 1, abs( nStride ), pFile );
		}
		delete []pBuffer;
		break;

	case colorModeRGB8:
	case colorModeBGR8:

		oFileHeader.bfType      = 'M' * 256 + 'B';
		oFileHeader.bfSize      = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + nHeight * nWidth * 4;
		oFileHeader.bfReserved1 = 0;
		oFileHeader.bfReserved2 = 0;
		oFileHeader.bfOffBits   = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

		oInfoHeader.biSize          = sizeof(BITMAPINFOHEADER);
		oInfoHeader.biWidth         = nWidth;
		oInfoHeader.biHeight        = nHeight;
		oInfoHeader.biPlanes        = 1;
		oInfoHeader.biBitCount      = 32;
		oInfoHeader.biCompression   = BI_RGB;
		oInfoHeader.biSizeImage     = nHeight * nWidth * 4;
		oInfoHeader.biXPelsPerMeter = 0;
		oInfoHeader.biYPelsPerMeter = 0;
		oInfoHeader.biClrUsed       = 0;
		oInfoHeader.biClrImportant  = 0;

		::fwrite( &oFileHeader,  1, sizeof(BITMAPFILEHEADER), pFile );
		::fwrite( &oInfoHeader,  1, sizeof(BITMAPINFOHEADER), pFile );

		nSize = nHeight * nWidth * 4;

		pBuffer = new BYTE[nSize];
		if ( !pBuffer )
		{
			return SErrorMemory; 
		}
		pBufPointer = pBuffer;
		pRow = m_pData;
		for ( int nY = 0; nY < nHeight; nY++ )
		{
			pCurPixel = pRow;
			for ( int nX = 0; nX < nWidth; nX++, pBufPointer += 4 )
			{
				if ( colorModeRGB8 == m_eMode )
				{
					pBufPointer[2] = (BYTE)pCurPixel[0];
					pBufPointer[1] = (BYTE)pCurPixel[1];
					pBufPointer[0] = (BYTE)pCurPixel[2];
				}
				else if ( colorModeBGR8 == m_eMode )
				{
					pBufPointer[2] = (BYTE)pCurPixel[2];
					pBufPointer[1] = (BYTE)pCurPixel[1];
					pBufPointer[0] = (BYTE)pCurPixel[0];
				}
				else
				{
					pBufPointer[0] = 0;
					pBufPointer[1] = 0;
					pBufPointer[2] = 0;
				}
				pBufPointer[3] = 0;
				pCurPixel += 3;
			}
			pRow += m_nStride;
		}
		pBufPointer = pBuffer;
		nStride = nWidth * 4;
		pBufPointer += (nHeight - 1) * nStride;
		nStride = -nStride;

		for( int nRow = nHeight; nRow > 0 ; --nRow, pBufPointer += nStride )
		{
			::fwrite( pBufPointer, 1, abs( nStride ), pFile );
		}
		delete []pBuffer;
		break;
	}

	fclose( pFile );
	return SNoError;
}

int SBitmap::WriteJPGFile(wchar_t *wsFileName, wchar_t *wsTempFolder)
{
	USES_CONVERSION;
	CStringW wsTempFileName = _T("");
	FILE *pTempFile = NULL;
	if ( !OpenTempFile( &wsTempFileName, &pTempFile, _T("wb"), _T(".bmp"), wsTempFolder ) ) 
	{
		return SErrorTemp;
	}
	::fclose( pTempFile );
	WriteBMPFile( wsTempFileName.GetBuffer() );
	AVSImageStudio::IImageTransforms *pTransform = NULL;
	CoCreateInstance( __uuidof( AVSImageStudio::ImageTransforms ), NULL, CLSCTX_INPROC_SERVER, __uuidof(AVSImageStudio::IImageTransforms), (void **)(&pTransform) );
	if ( !pTransform )
		return SErrorAVSImageStudio;
	CString sXml("<transforms><ImageFile-LoadImage sourcepath=\"");
	sXml += wsTempFileName.GetBuffer();
	sXml += "\"></ImageFile-LoadImage><ImageFile-SaveAsJpeg destinationpath=\"";
	sXml += wsFileName;
	sXml += "\" format=\"888\" quality=\"90\"></ImageFile-SaveAsJpeg></transforms>";
	BSTR bsXML = sXml.AllocSysString();

	VARIANT_BOOL vbRes = VARIANT_FALSE;
	pTransform->SetXml(bsXML, &vbRes);
	if ( vbRes == VARIANT_FALSE )
	{
		::SysFreeString( bsXML );
		RELEASEINTERFACE( pTransform );
		_wunlink( wsTempFileName.GetBuffer() );
		return SErrorAVSImageStudio;
	}
	vbRes = VARIANT_FALSE;
	pTransform->Transform(&vbRes);
	if ( vbRes == VARIANT_FALSE )
	{
		::SysFreeString( bsXML );
		RELEASEINTERFACE( pTransform );
		_wunlink( wsTempFileName.GetBuffer() );
		return SErrorAVSImageStudio;
	}

	::SysFreeString( bsXML );
	RELEASEINTERFACE( pTransform );
	_wunlink( wsTempFileName.GetBuffer() );
	return SNoError;
}
void SBitmap::GetPixel(int nX, int nY, SColorPointer pPixel) 
{
	SColorPointer pColor = NULL;

	if ( nY < 0 || nY >= m_nHeight || nX < 0 || nX >= m_nWidth ) 
	{
		return;
	}
	switch (m_eMode) 
	{
	case colorModeMono1:
		pColor = &m_pData[nY * m_nStride + (nX >> 3)];
		pPixel[0] = (pColor[0] & (0x80 >> (nX & 7))) ? 0xff : 0x00;
		break;
	case colorModeMono8:
		pColor = &m_pData[nY * m_nStride + nX];
		pPixel[0] = pColor[0];
		break;
	case colorModeRGB8:
		pColor = &m_pData[nY * m_nStride + 3 * nX];
		pPixel[0] = pColor[0];
		pPixel[1] = pColor[1];
		pPixel[2] = pColor[2];
		break;
	case colorModeBGR8:
		pColor = &m_pData[nY * m_nStride + 3 * nX];
		pPixel[0] = pColor[2];
		pPixel[1] = pColor[1];
		pPixel[2] = pColor[0];
		break;
	}
}

unsigned char SBitmap::GetAlpha(int nX, int nY) 
{
	return m_pAlpha[nY * m_nWidth + nX];
}
