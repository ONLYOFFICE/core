/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#include "BgraFrame.h"
#include "../common/File.h"
#include "../cximage/CxImage/ximage.h"
#include "ImageFileFormatChecker.h"
#include "../graphics/Image.h"

#if CXIMAGE_SUPPORT_JP2
#include "Jp2/J2kFile.h"
#include "JBig2/source/JBig2File.h"
#endif

#include <cmath>
#define BGRA_FRAME_CXIMAGE_MAX_MEMORY 67108864 // 256Mb (*4 channel)

void CxImageToMediaFrame( CxImage* img, CBgraFrame* bgra )
{
	if( !img || !img->IsValid() )
		return;

	int nWidth  = img->GetWidth();
	int nHeight = img->GetHeight();

	BYTE* pData = new BYTE[4 * nWidth * nHeight];

	if (!pData)
		return;

	bgra->put_Data(pData);
	bgra->put_Width(nWidth);
	bgra->put_Height(nHeight);
	bgra->put_Stride(-4 * nWidth);

	BYTE* pPixels = bgra->get_Data();

	int nBitsPerPixel = img->GetBpp();
	int nStride = img->GetEffWidth();
	BYTE* pBuffer = img->GetBits();
	RGBQUAD* pPalette = img->GetPalette();
	bool bIsAlphaPalettePresent = img->AlphaPaletteIsEnabled();
	bool bIsAlphaApplied = false;
	bool bIsRGBA = !bgra->get_IsRGBA();

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
				dst[0] = bIsRGBA ? pPalette[index].rgbBlue : pPalette[index].rgbRed;
				dst[1] = pPalette[index].rgbGreen;
				dst[2] = bIsRGBA ? pPalette[index].rgbRed : pPalette[index].rgbBlue;
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
					dst[0] = bIsRGBA ? pPalette[index].rgbBlue : pPalette[index].rgbRed;
					dst[1] = pPalette[index].rgbGreen;
					dst[2] = bIsRGBA ? pPalette[index].rgbRed : pPalette[index].rgbBlue;
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

				int nTransIndex = img->GetTransIndex();
				if (bIsAlphaApplied)
					nTransIndex = -1;

				for( int nRow = 0; nRow < nHeight; ++nRow, src += nStride )
				{
					for( int nPos = 0; nPos < nWidth; ++nPos, dst += 4 )
					{
						int index = (src[nPos >> 1] >> (1 - (nPos & 1)) * 4) & 15;
						dst[0] = bIsRGBA ? pPalette[index].rgbBlue : pPalette[index].rgbRed;
						dst[1] = pPalette[index].rgbGreen;
						dst[2] = bIsRGBA ? pPalette[index].rgbRed : pPalette[index].rgbBlue;

						if (bIsAlphaPalettePresent)
							dst[3] = pPalette[index].rgbReserved;
						else if (-1 != nTransIndex)
						{
							if (index == nTransIndex)
								dst[3] = pPalette[index].rgbReserved;
							else
								dst[3] = 255;
						}
					}
				}

				if (-1 != nTransIndex || bIsAlphaPalettePresent)
					bIsAlphaApplied = true;
			}
			else
				if( 8 == nBitsPerPixel )
				{
					BYTE* src = pBuffer;
					BYTE* dst = pPixels;

					nStride -= nWidth;

					int nTransIndex = img->GetTransIndex();
					if (bIsAlphaApplied)
						nTransIndex = -1;

					if( pPalette )
					{
						for( int nRow = 0; nRow < nHeight; ++nRow, src += nStride )
						{
							for( int nPos = 0; nPos < nWidth; ++nPos, src += 1, dst += 4 )
							{
								int index = src[0];
								dst[0] = bIsRGBA ? pPalette[index].rgbBlue : pPalette[index].rgbRed;
								dst[1] = pPalette[index].rgbGreen;
								dst[2] = bIsRGBA ? pPalette[index].rgbRed : pPalette[index].rgbBlue;

								if (bIsAlphaPalettePresent)
									dst[3] = pPalette[index].rgbReserved;
								else if (-1 != nTransIndex)
								{
									if (index == nTransIndex)
										dst[3] = pPalette[index].rgbReserved;
									else
										dst[3] = 255;
								}
							}
						}

						if (-1 != nTransIndex || bIsAlphaPalettePresent)
							bIsAlphaApplied = true;
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
							dst[0] = bIsRGBA ? src[0] : src[2];
							dst[1] = src[1];
							dst[2] = bIsRGBA ? src[2] : src[0];
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
							dst[0] = bIsRGBA ? src[0] : src[2];
							dst[1] = src[1];
							dst[2] = bIsRGBA ? src[2] : src[0];
						}
					}
				}
				else
				{
					bgra->Destroy();
					return;
				}

	if( img->AlphaIsValid() )
	{
		BYTE* pAlpha  = img->AlphaGetPointer();
		DWORD nMaxAlpha = img->AlphaGetMax();

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
				table[i] = (BYTE)((a > 255) ? 255 : a);
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

CBgraFrame::CBgraFrame()
{
	Clear();
}
CBgraFrame::~CBgraFrame()
{
	Destroy();
}

void CBgraFrame::Destroy()
{
	if (NULL != m_pData)
		delete []m_pData;

	if (NULL != m_pPalette)
		delete []m_pPalette;

	Clear();
}
void CBgraFrame::Clear()
{
	m_nFileType	= 0;
	m_lWidth	= 0;
	m_lHeight	= 0;
	m_lStride	= 0;
	m_pData		= NULL;
	m_pPalette  = NULL;
	m_lPaletteColors = 0;
	m_bIsGrayScale  = false;
	m_dJpegSaveQuality = -1;
	m_bIsRGBA = false;
}

void CBgraFrame::ClearNoAttack()
{
	Clear();
}
int CBgraFrame::get_Width()
{
	return m_lWidth;
}
int CBgraFrame::get_Height()
{
	return m_lHeight;
}
void CBgraFrame::put_Width(const int& lWidth)
{
	m_lWidth = lWidth;
}
void CBgraFrame::put_Height(const int& lHeight)
{
	m_lHeight = lHeight;
}
int CBgraFrame::get_Stride()
{
	return m_lStride;
}
void CBgraFrame::put_Stride(const int& lStride)
{
	m_lStride = lStride;
}
BYTE* CBgraFrame::get_Data()
{
	return m_pData;
}
void CBgraFrame::put_Data(BYTE* pData)
{
	m_pData = pData;
}

bool CBgraFrame::IsGrayScale()
{
	return m_bIsGrayScale;
}

void CBgraFrame::SetJpegQuality(const double& value)
{
	m_dJpegSaveQuality = value;
}

void CBgraFrame::put_IsRGBA(const bool& bIsRGBA)
{
	m_bIsRGBA = bIsRGBA;
}
bool CBgraFrame::get_IsRGBA()
{
	return m_bIsRGBA;
}

void CBgraFrame::put_Palette(BYTE* pDataColors, const int& colors)
{
	if (!pDataColors || colors < 1) return;

	if (m_pPalette)
		delete []m_pPalette;

	m_lPaletteColors = colors;
	m_pPalette = new BYTE[colors * 4];

	memcpy(m_pPalette, pDataColors, colors * 4);
}

bool CBgraFrame::OpenFile(const std::wstring& strFileName, unsigned int nFileType)
{
	m_nFileType = nFileType;

	if (nFileType == 0)
	{
		CImageFileFormatChecker checker(strFileName);
		m_nFileType = checker.eFileType;
	}

#if CXIMAGE_SUPPORT_JP2
	if (CXIMAGE_FORMAT_JP2 == m_nFileType)
	{
		Jpeg2000::CJ2kFile oJ2;
		return oJ2.Open(this, strFileName, std::wstring(L""), !m_bIsRGBA);
	}
#endif

	NSFile::CFileBinary oFile;
	if (!oFile.OpenFile(strFileName))
		return false;

	CxImage* img = new CxImage();

	if (!img->Decode(oFile.GetFileNative(), m_nFileType))
		return false;

	CxImage* imgResample = NULL;

	if (false)
	{
		// slow!!!
		int nWidth  = img->GetWidth();
		int nHeight = img->GetHeight();

		double dSizeWH = (double)nWidth * nHeight;
		double dSizeLimit = (double)BGRA_FRAME_CXIMAGE_MAX_MEMORY;
		if (dSizeWH > dSizeLimit)
		{
			double dKoef = sqrt(dSizeLimit / dSizeWH);
			int nW = (int)(dKoef * nWidth);
			int nH = (int)(dKoef * nHeight);

			if (nW > 10 && nH > 10)
			{
				imgResample = new CxImage();
				if (!img->Resample(nW, nH, 2/*bicubic spline interpolation*/, imgResample))
				{
					delete imgResample;
					imgResample = NULL;
				}
			}
		}
	}

	CxImage* imageFinal = img;
	if (imgResample)
	{
		delete img;
		imageFinal = imgResample;
	}

	CxImageToMediaFrame(imageFinal, this);
	m_bIsGrayScale = imageFinal->IsGrayScale();
	delete imageFinal;

	return true;
}
bool CBgraFrame::Decode(BYTE* pBuffer, int nSize, unsigned int nFileType)
{
	m_nFileType = nFileType;

	if (nFileType == 0)
	{
		CImageFileFormatChecker checker(pBuffer, nSize);
		m_nFileType = checker.eFileType;
	}

#if CXIMAGE_SUPPORT_JP2
	if (CXIMAGE_FORMAT_JP2 == m_nFileType)
	{
		Jpeg2000::CJ2kFile oJ2;
		return oJ2.Open(this, pBuffer, nSize, std::wstring(L""), !m_bIsRGBA);
	}
#endif

	CxImage img;

	if (!img.Decode(pBuffer, nSize, m_nFileType))
		return false;

	CxImageToMediaFrame(&img, this);
	m_bIsGrayScale = img.IsGrayScale();
	return true;
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

#if CXIMAGE_SUPPORT_JP2
	if (21/*CXIMAGE_FORMAT_JBIG2*/ == nFileType)
	{
		CJBig2File jBig2File;
		bool res = jBig2File.MemoryToJBig2(m_pData, m_lWidth * m_lHeight * 24, m_lWidth, m_lHeight, strFileName, !m_bIsRGBA);

		return res;
	}
	else
#endif
	{
		NSFile::CFileBinary oFile;
		if (!oFile.CreateFileW(strFileName))
			return false;
		
		CxImage img;

		if (!img.CreateFromArray(m_pData, m_lWidth, m_lHeight, lBitsPerPixel * 8, lStride, (m_lStride >= 0) ? true : false, !m_bIsRGBA))
			return false;

		if (m_pPalette)
		{
			img.SetPalette((RGBQUAD*)m_pPalette, m_lPaletteColors);
		}

		if (!img.Encode(oFile.GetFileNative(), nFileType))
			return false;

		oFile.CloseFile();
	}
	return true;
}
bool CBgraFrame::Encode(BYTE*& pBuffer, int& nSize, unsigned int nFileType)
{
	CxImage oCxImage;
	if (!oCxImage.CreateFromArray(m_pData, m_lWidth, m_lHeight, 32, 4 * m_lWidth, (m_lStride >= 0) ? true : false))
		return false;

	if (CXIMAGE_FORMAT_JPG == nFileType && -1 != m_dJpegSaveQuality)
		oCxImage.SetJpegQualityF((float)m_dJpegSaveQuality);

	return oCxImage.Encode(pBuffer, nSize, nFileType);
}
void CBgraFrame::FreeEncodedMemory(void* pMemory)
{
    CxImage oCxImage;
    oCxImage.FreeMemory(pMemory);
}
bool CBgraFrame::Resize(const long& nNewWidth, const long& nNewHeight, bool bDestroyData)
{
	CxImage img;
	if (!img.CreateFromArray(m_pData, m_lWidth, m_lHeight, 32, 4 * m_lWidth, (m_lStride >= 0) ? true : false))
		return false;

	CxImage imgDst;
	if (!img.Resample( nNewWidth, nNewHeight, 2/*bicubic spline interpolation*/, &imgDst ))
		return false;

	if (bDestroyData)
		Destroy();

	CxImageToMediaFrame( &imgDst, this );
	return true;
}
bool CBgraFrame::ReColorPatternImage(const std::wstring& strFileName, unsigned int rgbColorBack, unsigned int rgbColorFore)
{
	if (OpenFile(strFileName))
	{
		int smpl = abs(get_Stride() / get_Width());

		BYTE * rgb = get_Data();
		
		BYTE R1 = (BYTE)(rgbColorBack);
		BYTE G1 = (BYTE)(rgbColorBack >> 8);
		BYTE B1 = (BYTE)(rgbColorBack >> 16);

		BYTE R2 = (BYTE)(rgbColorFore);
		BYTE G2 = (BYTE)(rgbColorFore >> 8);
		BYTE B2 = (BYTE)(rgbColorFore >> 16);

		for (int i = 0 ; i < get_Width() * get_Height(); i++)
		{
			if (rgb[i * smpl + 0 ] == 0x00 && rgb[i * smpl + 1 ] == 0x00 && rgb[i * smpl + 2 ] == 0x00)
			{
				rgb[i * smpl + 0 ] = R1;
				rgb[i * smpl + 1 ] = G1;
				rgb[i * smpl + 2 ] = B1;
			}
			else
			{
				rgb[i * smpl + 0 ] = R2;
				rgb[i * smpl + 1 ] = G2;
				rgb[i * smpl + 2 ] = B2;
			}
		}
		if (m_nFileType == 0) m_nFileType = 1;

		SaveFile(strFileName, m_nFileType);
		return true;
	}
	return false;
}

void CBgraFrame::FromImage(IGrObject* pGrObject, bool bIsCopy)
{
	Aggplus::CImage* pImage = (Aggplus::CImage*)pGrObject;

	this->put_Width((int)pImage->GetWidth());
	this->put_Height((int)pImage->GetHeight());
	this->put_Stride((int)pImage->GetStride());

	int nSize = 4 * m_lWidth * m_lHeight;

	if (nSize > 0)
	{
		if (bIsCopy)
		{
			m_pData = new BYTE[nSize];
			memcpy(m_pData, pImage->GetData(), nSize);
		}
		else
		{
			m_pData = pImage->GetData();
		}
	}
}
