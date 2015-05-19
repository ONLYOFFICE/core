#include "BgraFrame.h"
#include "../common/File.h"
#include "../cximage/CxImage/ximage.h"

#include "Jp2/J2kFile.h"
#include "JBig2/source/JBig2File.h"
#include "SvmFile/source/SvmFile.h"

bool CBgraFrame::OpenFile(const std::wstring& strFileName, unsigned int nFileType)
{
    if (CXIMAGE_FORMAT_JP2 == nFileType)
    {
        Jpeg2000::CJ2kFile oJ2;
        return oJ2.Open(this, strFileName, std::wstring(L""));
    }
    if (22/*CXIMAGE_FORMAT_SVM*/ == nFileType)
    {
        SvmFile::CSvmFile oSvm;
        return oSvm.Open(this, strFileName);
    }
	else
	{
		NSFile::CFileBinary oFile;
		if (!oFile.OpenFile(strFileName))
			return false;

		CxImage img;

		if (!img.Decode(oFile.GetFileNative(), nFileType))
			return false;

		CxImageToMediaFrame(img);
		return true;
	}
}

bool CBgraFrame::SaveFile(const std::wstring& strFileName, unsigned int nFileType)
{
	uint32_t lStride = 4 * m_lWidth;
	uint32_t lBitsPerPixel = 4;
	if (0 != m_lStride)
	{
		lStride = (m_lStride > 0) ? (uint32_t)m_lStride : (uint32_t)(-m_lStride);
		lBitsPerPixel = lStride / m_lWidth;
	}

	if (21/*CXIMAGE_FORMAT_JBIG2*/ == nFileType)
	{
		CJBig2File jBig2File;
		bool res = jBig2File.MemoryToJBig2(m_pData, m_lWidth * m_lHeight * 24, m_lWidth, m_lHeight, strFileName);

		return res;		
	}
	else
	{
		NSFile::CFileBinary oFile;
		if (!oFile.CreateFileW(strFileName))
			return false;
		
		CxImage img;
		if (!img.CreateFromArray(m_pData, m_lWidth, m_lHeight, lBitsPerPixel * 8, lStride, (m_lStride >= 0) ? true : false))
			return false;

		if (!img.Encode(oFile.GetFileNative(), nFileType))
			return false;

		oFile.CloseFile();
	}
	return true;
}
bool CBgraFrame::Resize(const long& nNewWidth, const long& nNewHeight)
{
	CxImage img;
	if (!img.CreateFromArray(m_pData, m_lWidth, m_lHeight, 32, 4 * m_lWidth, (m_lStride >= 0) ? true : false))
		return false;

	CxImage imgDst;
	if (!img.Resample( nNewWidth, nNewHeight, 2/*bicubic spline interpolation*/, &imgDst ))
		return false;

	CxImageToMediaFrame( imgDst );
	return true;
}

void CBgraFrame::CxImageToMediaFrame( CxImage& img )
{
	if( !img.IsValid() )
		return;
	
	int nWidth  = img.GetWidth();
	int nHeight = img.GetHeight();
	
	m_pData = new BYTE[4 * nWidth * nHeight];

	if (!m_pData)
		return;

	m_lWidth = nWidth;
	m_lHeight = nHeight;
	m_lStride = -4 * nWidth;
	
	BYTE* pPixels = m_pData;

	int nBitsPerPixel = img.GetBpp();
	int nStride = img.GetEffWidth();
	BYTE* pBuffer = img.GetBits();
	RGBQUAD* pPalette = img.GetPalette();
	bool bIsAlphaPalettePresent = false;//img.GetPaletteAlphaEnabled();
	bool bIsAlphaApplied = false;

	if( 1 == nBitsPerPixel )
	{
		RGBQUAD pal[2];

		if( !pPalette )
		{
			for( int i = 0; i < 2; i++ )
			{
				int c = i * 255;
				pal[i].rgbBlue  = c;
				pal[i].rgbGreen = c;
				pal[i].rgbRed   = c;
			}
			pPalette = pal;
		}

		BYTE* src = pBuffer;
		BYTE* dst = pPixels;

		for( int nRow = 0; nRow < nHeight; ++nRow, src += nStride )
		{
			for( int nPos = 0; nPos < nWidth; ++nPos, dst += 4 )
			{
				int index = (src[nPos >> 3] >> (7 - (nPos & 7)) * 1) & 1;
				dst[0] = pPalette[index].rgbBlue;
				dst[1] = pPalette[index].rgbGreen;
				dst[2] = pPalette[index].rgbRed;
			}
		}
	}
	else
	if( 2 == nBitsPerPixel )
	{
		RGBQUAD pal[4];

		if( !pPalette )
		{
			for( int i = 0; i < 4; i++ )
			{
				int c = (i * 255 + 2) / 3;
				pal[i].rgbBlue  = c;
				pal[i].rgbGreen = c;
				pal[i].rgbRed   = c;
			}
			pPalette = pal;
		}

		BYTE* src = pBuffer;
		BYTE* dst = pPixels;

		for( int nRow = 0; nRow < nHeight; ++nRow, src += nStride )
		{
			for( int nPos = 0; nPos < nWidth; ++nPos, dst += 4 )
			{
				int index = (src[nPos >> 2] >> (3 - (nPos & 3)) * 2) & 3;
				dst[0] = pPalette[index].rgbBlue;
				dst[1] = pPalette[index].rgbGreen;
				dst[2] = pPalette[index].rgbRed;
			}
		}
	}
	else
	if( 4 == nBitsPerPixel )
	{
		RGBQUAD pal[16];

		if( !pPalette )
		{
			for( int i = 0; i < 16; i++ )
			{
				int c = (i * 255 + 8) / 15;
				pal[i].rgbBlue  = c;
				pal[i].rgbGreen = c;
				pal[i].rgbRed   = c;
			}
			pPalette = pal;
		}

		BYTE* src = pBuffer;
		BYTE* dst = pPixels;

		for( int nRow = 0; nRow < nHeight; ++nRow, src += nStride )
		{
			for( int nPos = 0; nPos < nWidth; ++nPos, dst += 4 )
			{
				int index = (src[nPos >> 1] >> (1 - (nPos & 1)) * 4) & 15;
				dst[0] = pPalette[index].rgbBlue;
				dst[1] = pPalette[index].rgbGreen;
				dst[2] = pPalette[index].rgbRed;
			}
		}
	}
	else
	if( 8 == nBitsPerPixel )
	{
		BYTE* src = pBuffer;
		BYTE* dst = pPixels;

		nStride -= nWidth;

		if( pPalette )
		{
			if (bIsAlphaPalettePresent)
			{
				for( int nRow = 0; nRow < nHeight; ++nRow, src += nStride )
				{
					for( int nPos = 0; nPos < nWidth; ++nPos, src += 1, dst += 4 )
					{
						int index = src[0];
						dst[0] = pPalette[index].rgbBlue;
						dst[1] = pPalette[index].rgbGreen;
						dst[2] = pPalette[index].rgbRed;
						dst[3] = pPalette[index].rgbReserved;
					}
				}
				bIsAlphaApplied = true;
			}
			else
			{
				for( int nRow = 0; nRow < nHeight; ++nRow, src += nStride )
				{
					for( int nPos = 0; nPos < nWidth; ++nPos, src += 1, dst += 4 )
					{
						int index = src[0];
						dst[0] = pPalette[index].rgbBlue;
						dst[1] = pPalette[index].rgbGreen;
						dst[2] = pPalette[index].rgbRed;								
					}
				}
			}
		}
		else
		{
			for( int nRow = 0; nRow < nHeight; ++nRow, src += nStride )
			{
				for( int nPos = 0; nPos < nWidth; ++nPos, src += 1, dst += 4 )
				{
					int index = src[0];
					dst[0] = index;
					dst[1] = index;
					dst[2] = index;
				}
			}
		}
	}
	else if( 24 == nBitsPerPixel )
	{
		BYTE* src = pBuffer;
		BYTE* dst = pPixels;

		nStride -= nWidth * 3;

		for( int nRow = 0; nRow < nHeight; ++nRow, src += nStride )
		{
			for( int nPos = 0; nPos < nWidth; ++nPos, src += 3, dst += 4 )
			{
				dst[0] = src[0];
				dst[1] = src[1];
				dst[2] = src[2];
			}
		}
	}
	else if( 32 == nBitsPerPixel )
	{
		BYTE* src = pBuffer;
		BYTE* dst = pPixels;

		nStride -= nWidth * 4;

		for( int nRow = 0; nRow < nHeight; ++nRow, src += nStride )
		{
			for( int nPos = 0; nPos < nWidth; ++nPos, src += 4, dst += 4 )
			{
				dst[0] = src[0];
				dst[1] = src[1];
				dst[2] = src[2];
			}
		}
	}
	else
	{
		Destroy();
		return;
	}
	
	if( img.AlphaIsValid() )
	{
		BYTE* pAlpha  = img.AlphaGetPointer();
		DWORD nMaxAlpha = img.AlphaGetMax();

		if( 255 == nMaxAlpha )
		{
			BYTE* src = pAlpha;
			BYTE* dst = pPixels;
			int nSize = nWidth * nHeight;

			for( int i = 0; i < nSize; ++i, ++src, dst += 4 )
			{
				dst[3] = src[0];
			}
		}
		else
		{
			BYTE table[256];
			for( DWORD i = 0; i < 256; ++i )
			{
				DWORD a = (i * 255 + nMaxAlpha / 2) / nMaxAlpha;
                table[i] = (a > 255) ? 255 : a;
			}

			BYTE* src = pAlpha;
			BYTE* dst = pPixels;
			int nSize = nWidth * nHeight;

			for( int i = 0; i < nSize; ++i, ++src, dst += 4 )
			{
				dst[3] = table[src[0]];
			}
		}
	}
	else if (!bIsAlphaApplied)
	{
		BYTE* dst = pPixels;
		int nSize = nWidth * nHeight;

		for( int i = 0; i < nSize; ++i, dst += 4 )
		{
			dst[3] = 255;
		}
	}		
}
