/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#include "MetaFileUtils.h"

#include "../../ImageFileFormatChecker.h"
#include "../../BgraFrame.h"

#include <time.h>
#include <math.h>

#ifndef DIB_RGB_COLORS
    #define DIB_RGB_COLORS  0x00
#endif

namespace MetaFile
{
	unsigned char GetLowestBit(unsigned int ulValue)
	{
		if (0 == ulValue)
			return 0;

		unsigned char unOffset = 0;
		unsigned int ulBit = 1;
		while (!(ulValue & ulBit))
		{
			ulBit = ulBit << 1;
			unOffset++;

			// ограничиваемся 32-битами
			if (ulBit & 0x80000000)
				return 0;
		}

		return unOffset;
	}
	bool ReadImageCoreHeader(BYTE* pHeaderBuffer, unsigned int ulHeaderBufferLen, BYTE* pImageBuffer, unsigned int ulImageBufferLen, BYTE** ppDstBuffer, unsigned int* pulWidth, unsigned int* pulHeight)
	{
		CDataStream oHeaderStream;
		oHeaderStream.SetStream(pHeaderBuffer, ulHeaderBufferLen);

		unsigned short ushWidth;
		unsigned short ushHeight;
		unsigned short ushPlanes;
		unsigned short ushBitCount;

		oHeaderStream >> ushWidth;
		oHeaderStream >> ushHeight;
		oHeaderStream >> ushPlanes;
		oHeaderStream >> ushBitCount;

		if (0x0001 != ushPlanes)
			return false;

		return false;
	}
	bool ReadImageInfoHeader(BYTE* pHeaderBuffer, unsigned int ulHeaderBufferLen, BYTE* pImageBuffer, unsigned int ulImageBufferLen, BYTE** ppDstBuffer, unsigned int* pulWidth, unsigned int* pulHeight)
	{
		CDataStream oHeaderStream;
		oHeaderStream.SetStream(pHeaderBuffer, ulHeaderBufferLen);

		int nWidth;
		int nHeight;

		unsigned short ushPlanes;
		unsigned short ushBitCount;

		unsigned int unCompression;
		unsigned int unImageSize;
		unsigned int unXPelsPerMeter;
		unsigned int unYPelsPerMeter;
		unsigned int unColorUsed;
		unsigned int unColorImportant;

		oHeaderStream >> nWidth;
		oHeaderStream >> nHeight;
		oHeaderStream >> ushPlanes;
		oHeaderStream >> ushBitCount;
		oHeaderStream >> unCompression;
		oHeaderStream >> unImageSize;
		oHeaderStream >> unXPelsPerMeter;
		oHeaderStream >> unYPelsPerMeter;
		oHeaderStream >> unColorUsed;
		oHeaderStream >> unColorImportant;

		if (0x0001 != ushPlanes)
			return false;

		if (nHeight < 0x00000000 && (BI_RGB != unCompression))
			return false;

		if (nWidth < 0)
			return false;

		BYTE* pBgraBuffer = NULL;
		unsigned int ulWidth  = 0;
		unsigned int ulHeight = 0;

		BYTE* pBuffer = pImageBuffer;
		int  lBufLen = ulImageBufferLen;

		*ppDstBuffer = NULL;
		*pulWidth    = 0;
		*pulHeight   = 0;

		if (BI_BITCOUNT_0 == ushBitCount) // Значит компрессия либо PNG, либо JPEG
		{
			if (BI_JPEG != unCompression || BI_PNG != unCompression)
				return false;

			std::wstring wsTempFileName;
			FILE* pTempFile = NULL;
			if (!OpenTempFile(&wsTempFileName, &pTempFile, L"wb", L".wmf0", NULL))
				return false;

			::fwrite(pBuffer, 1, unImageSize, pTempFile);
			::fclose(pTempFile);

			CBgraFrame oFrame;
			oFrame.OpenFile(wsTempFileName);

			// TODO: Как будут файлы сделать чтение.
			NSFile::CFileBinary::Remove(wsTempFileName);
			return false;
		}
		else if (BI_BITCOUNT_1 == ushBitCount)
		{
			// Двуцветная картинка, значит палитра состоит из 2-х цветов
			TRgbQuad oColor1, oColor2;

			if (oHeaderStream.CanRead() >= 8)
				oHeaderStream >> oColor1 >> oColor2;

			// Считываем саму картинку
			int lCalcLen = (((nWidth * ushPlanes * ushBitCount + 31) & ~31) / 8) * abs(nHeight);
			if (lCalcLen > lBufLen)
				return false;

			pBgraBuffer = new BYTE[nWidth * abs(nHeight) * 4 * sizeof(BYTE)];
			if (NULL == pBgraBuffer)
				return false;

			ulHeight = (unsigned short)abs(nHeight);
			ulWidth  = (unsigned short)nWidth;

			int nWidthBytes = (nWidth + 7) / 8;
			int nAdditBytes = 4 - div_t(div(((nWidth + 7) / 8), 4)).rem;
			if (4 == nAdditBytes)
				nAdditBytes = 0;

			int nLastBitCount = div_t(div(nWidth, 8)).rem - 1;
			if (-1 == nLastBitCount)
				nLastBitCount = 7;

			nLastBitCount = (int)pow((double)2, (double)nLastBitCount);

			if (nHeight < 0)
			{
				for (int nY = 0; nY < abs(nHeight); nY++)
				{
					int nIndex = 4 * nWidth * nY;
					for (int nX = 0; nX < nWidthBytes; nX++)
					{
						int nByte = *pBuffer; pBuffer++; lBufLen--;
						int nBitCount = 128;
						int nAlpha = 255;
						if (nX == nWidthBytes - 1)
						{
							// Не до конца заполненный байт иногда заполняется странным цветом, поэтому мы делаем его прозрачным
							nBitCount = nLastBitCount;
							nAlpha = 0;
						}

						for (int nBitIndex = nBitCount; nBitIndex > 0; nBitIndex /= 2)
						{
							int nBit = (nByte & nBitIndex);
							TRgbQuad* pColor = (nBit ? &oColor2 : &oColor1);
							pBgraBuffer[nIndex + 0] = pColor->b;
							pBgraBuffer[nIndex + 1] = pColor->g;
							pBgraBuffer[nIndex + 2] = pColor->r;
							pBgraBuffer[nIndex + 3] = nAlpha;
							nIndex += 4;
						}
					}
					for (int nAddIndex = 0; nAddIndex < nAdditBytes; nAddIndex++)
					{
						int nByte = *pBuffer; pBuffer++; lBufLen--;
					}
				}
			}
			else
			{
				for (int nY = abs(nHeight) - 1; nY >= 0; nY--)
				{
					int nIndex = 4 * nWidth * nY;
					for (int nX = 0; nX < nWidthBytes; nX++)
					{
						int nByte = *pBuffer; pBuffer++; lBufLen--;
						int nBitCount = 128;
						int nAlpha = 255;
						if (nX == nWidthBytes - 1)
						{
							// Не до конца заполненный байт иногда заполняется странным цветом, поэтому мы делаем его прозрачным
							nBitCount = nLastBitCount;
							//nAlpha = 0;
						}

						for (int nBitIndex = nBitCount; nBitIndex > 0; nBitIndex /= 2)
						{
							int nBit = (nByte & nBitIndex);
							TRgbQuad* pColor = (nBit ? &oColor2 : &oColor1);
							pBgraBuffer[nIndex + 0] = pColor->b;
							pBgraBuffer[nIndex + 1] = pColor->g;
							pBgraBuffer[nIndex + 2] = pColor->r;
							pBgraBuffer[nIndex + 3] = nAlpha;
							nIndex += 4;
						}
					}
					for (int nAddIndex = 0; nAddIndex < nAdditBytes; nAddIndex++)
					{
						int nByte = *pBuffer; pBuffer++; lBufLen--;
					}
				}
			}

			*ppDstBuffer = pBgraBuffer;
			*pulWidth    = ulWidth;
			*pulHeight   = ulHeight;

			return true;
		}
		else if (BI_BITCOUNT_2 == ushBitCount)
		{
			unsigned char unColorTableLen = 16;
			if (0 != unColorUsed)
				unColorTableLen = (std::min)((unsigned char)16, (unsigned char)unColorUsed);

			TRgbQuad oColorTable[16];
			if (oHeaderStream.CanRead() < unColorTableLen * 4)
				return false;

			// Считываем палитру
			for (unsigned short ushIndex = 0; ushIndex < unColorTableLen; ushIndex++)
			{
				oHeaderStream >> oColorTable[ushIndex];
			}

			// 4 бита - 1 пиксел

			// Считываем саму картинку
			int lCalcLen = (((nWidth * ushPlanes * ushBitCount + 31) & ~31) / 8) * abs(nHeight);
			if (lCalcLen != lBufLen)
				return false;

			// Ширина в байтах должна быть кратна 4, значит сама ширина должна быть кратна 8
			int nAdd = 0;
			while (0 != div_t(div(nWidth + nAdd, 8)).rem)
			{
				nAdd++;
			}

			int nScanLineBytes = (nWidth + nAdd) / 2;
			if (lBufLen < (nScanLineBytes * abs(nHeight)))
				return false;

			pBgraBuffer = new BYTE[nWidth * abs(nHeight) * 4 * sizeof(BYTE)];
			if (NULL == pBgraBuffer)
				return false;

			ulHeight = (unsigned short)abs(nHeight);
			ulWidth  = (unsigned short)nWidth;

			if (nHeight < 0)
			{
				for (int nY = 0; nY < abs(nHeight); nY++)
				{
					for (int nLineIndex = 0; nLineIndex < nScanLineBytes; nLineIndex++)
					{
						BYTE nByte = *pBuffer; pBuffer++;

						int nX = nLineIndex * 2;
						int nIndex = 4 * (nWidth * nY + nX);
						BYTE nColorIndex = (nByte & 0xf0) >> 4;

						if (nX < nWidth)
						{
							pBgraBuffer[nIndex + 0] = oColorTable[nColorIndex].b;
							pBgraBuffer[nIndex + 1] = oColorTable[nColorIndex].g;
							pBgraBuffer[nIndex + 2] = oColorTable[nColorIndex].r;
							pBgraBuffer[nIndex + 3] = 255;
						}

						nX = nLineIndex * 2 + 1;
						nIndex = 4 * (nWidth * nY + nX);
						nColorIndex = nByte & 0x0f;

						if (nX < nWidth)
						{
							pBgraBuffer[nIndex + 0] = oColorTable[nColorIndex].b;
							pBgraBuffer[nIndex + 1] = oColorTable[nColorIndex].g;
							pBgraBuffer[nIndex + 2] = oColorTable[nColorIndex].r;
							pBgraBuffer[nIndex + 3] = 255;
						}
					}
				}
			}
			else
			{
				for (int nY = abs(nHeight) - 1; nY >= 0; nY--)
				{
					for (int nLineIndex = 0; nLineIndex < nScanLineBytes; nLineIndex++)
					{
						BYTE nByte = *pBuffer; pBuffer++;

						int nX = nLineIndex * 2;
						int nIndex = 4 * (nWidth * nY + nX);
						BYTE nColorIndex = (nByte & 0xf0) >> 4;

						if (nX < nWidth)
						{
							pBgraBuffer[nIndex + 0] = oColorTable[nColorIndex].b;
							pBgraBuffer[nIndex + 1] = oColorTable[nColorIndex].g;
							pBgraBuffer[nIndex + 2] = oColorTable[nColorIndex].r;
							pBgraBuffer[nIndex + 3] = 255;
						}

						nX = nLineIndex * 2 + 1;
						nIndex = 4 * (nWidth * nY + nX);
						nColorIndex = nByte & 0x0f;

						if (nX < nWidth)
						{
							pBgraBuffer[nIndex + 0] = oColorTable[nColorIndex].b;
							pBgraBuffer[nIndex + 1] = oColorTable[nColorIndex].g;
							pBgraBuffer[nIndex + 2] = oColorTable[nColorIndex].r;
							pBgraBuffer[nIndex + 3] = 255;
						}
					}
				}
			}

			*ppDstBuffer = pBgraBuffer;
			*pulWidth    = ulWidth;
			*pulHeight   = ulHeight;

			return true;
		}
		else if (BI_BITCOUNT_3 == ushBitCount)
		{
			unsigned short ushColorTableLen = 256;

			if (0 != unColorUsed)
				ushColorTableLen = (std::min)((unsigned short)256, (unsigned short)unColorUsed);

			TRgbQuad oColorTable[256];
			if (oHeaderStream.CanRead() < ushColorTableLen * 4)
				return false;

			// Считываем палитру
			for (unsigned short ushIndex = 0; ushIndex < ushColorTableLen; ushIndex++)
			{
				oHeaderStream >> oColorTable[ushIndex];
			}

			// 1 байт - 1 пиксел
			// Ширина должна быть кратна 4.
			int nAdd = 0;
			while (0 != div_t(div(nWidth + nAdd, 4)).rem)
			{
				nAdd++;
			}

			if (lBufLen < (nWidth + nAdd) * abs(nHeight))
				return false;

			pBgraBuffer = new BYTE[nWidth * abs(nHeight) * 4 * sizeof(BYTE)];
			if (NULL == pBgraBuffer)
				return false;

			ulHeight = (unsigned short)abs(nHeight);
			ulWidth  = (unsigned short)nWidth;

			if (nHeight < 0)
			{
				for (int nY = 0; nY < abs(nHeight); nY++)
				{
					for (int nX = 0; nX < nWidth; nX++)
					{
						int nIndex = 4 * (nWidth * nY + nX);

						BYTE nByte = *pBuffer; pBuffer++; lBufLen--;

						pBgraBuffer[nIndex + 0] = oColorTable[nByte].b;
						pBgraBuffer[nIndex + 1] = oColorTable[nByte].g;
						pBgraBuffer[nIndex + 2] = oColorTable[nByte].r;
						pBgraBuffer[nIndex + 3] = 255;
					}
					pBuffer += nAdd; lBufLen -= nAdd;
				}
			}
			else
			{
				for (int nY = abs(nHeight) - 1; nY >= 0; nY--)
				{
					for (int nX = 0; nX < nWidth; nX++)
					{
						int nIndex = 4 * (nWidth * nY + nX);

						BYTE nByte = *pBuffer; pBuffer++; lBufLen--;

						pBgraBuffer[nIndex + 0] = oColorTable[nByte].b;
						pBgraBuffer[nIndex + 1] = oColorTable[nByte].g;
						pBgraBuffer[nIndex + 2] = oColorTable[nByte].r;
						pBgraBuffer[nIndex + 3] = 255;
					}
					pBuffer += nAdd; lBufLen -= nAdd;
				}
			}

			*ppDstBuffer = pBgraBuffer;
			*pulWidth    = ulWidth;
			*pulHeight   = ulHeight;

			return true;
		}
		else if (BI_BITCOUNT_4 == ushBitCount)
		{
			unsigned int ulMaskR = 0x1f, ulMaskB = 0x7C00, ulMaskG = 0x3E0;
			unsigned int ulShiftR = 0, ulShiftB = 10, ulShiftG = 5;
			double dKoefR = 255 / 31.0, dKoefB = 255 / 31.0, dKoefG = 255 / 31.0;

			if (BI_RGB == unCompression)
			{
				// Маски, сдвиги и коэффициенты уже заполнены стандартными значениями для масок
				// 000000000011111 - Red
				// 000001111100000 - Green
				// 111110000000000 - Blue
			}
			else if (BI_BITFIELDS == unCompression)
			{
				oHeaderStream >> ulMaskB;
				oHeaderStream >> ulMaskG;
				oHeaderStream >> ulMaskR;

				ulShiftR = GetLowestBit(ulMaskR);
				ulShiftB = GetLowestBit(ulMaskB);
				ulShiftG = GetLowestBit(ulMaskG);

				dKoefR = 255.0 / (ulMaskR >> ulShiftR);
				dKoefG = 255.0 / (ulMaskG >> ulShiftG);
				dKoefB = 255.0 / (ulMaskB >> ulShiftB);
			}
			else
				return false;

			// Считываем саму картинку
			int lCalcLen = (((nWidth * ushPlanes * ushBitCount + 31) & ~31) / 8) * abs(nHeight);
			if (lCalcLen != lBufLen)
				return false;

			// 2 байт на все каналы канал
			// (Ширина * 3) должна быть кратна 4.
			int nAdd = 0;
			while (0 != div_t(div(2 * nWidth + nAdd, 4)).rem)
			{
				nAdd++;
			}

			pBgraBuffer = new BYTE[nWidth * abs(nHeight) * 4 * sizeof(BYTE)];
			if (NULL == pBgraBuffer)
				return false;

			ulHeight = (unsigned short)abs(nHeight);
			ulWidth  = (unsigned short)nWidth;

			if (nHeight < 0)
			{
				for (int nY = 0; nY < abs(nHeight); nY++)
				{
					for (int nX = 0; nX < nWidth; nX++)
					{
						int nIndex = 4 * (nWidth * nY + nX);

						unsigned short ushValue = ((pBuffer[1] << 8) | pBuffer[0]) & 0xFFFF; pBuffer += 2; lBufLen -= 2;

						unsigned char unR = (ushValue & ulMaskR) >> ulShiftR;
						unsigned char unG = (ushValue & ulMaskG) >> ulShiftG;
						unsigned char unB = (ushValue & ulMaskB) >> ulShiftB;

						pBgraBuffer[nIndex + 0] = (unsigned char)(unR * dKoefR);
						pBgraBuffer[nIndex + 1] = (unsigned char)(unG * dKoefG);
						pBgraBuffer[nIndex + 2] = (unsigned char)(unB * dKoefB);
						pBgraBuffer[nIndex + 3] = 255;
					}
					pBuffer += nAdd; lBufLen -= nAdd;
				}
			}
			else
			{
				for (int nY = abs(nHeight) - 1; nY >= 0; nY--)
				{
					for (int nX = 0; nX < nWidth; nX++)
					{
						int nIndex = 4 * (nWidth * nY + nX);

						unsigned short ushValue = ((pBuffer[1] << 8) | pBuffer[0]) & 0xFFFF; pBuffer += 2; lBufLen -= 2;
						unsigned char unR = (ushValue & ulMaskR) >> ulShiftR;
						unsigned char unG = (ushValue & ulMaskG) >> ulShiftG;
						unsigned char unB = (ushValue & ulMaskB) >> ulShiftB;

						pBgraBuffer[nIndex + 0] = (unsigned char)(unR * dKoefR);
						pBgraBuffer[nIndex + 1] = (unsigned char)(unG * dKoefG);
						pBgraBuffer[nIndex + 2] = (unsigned char)(unB * dKoefB);
						pBgraBuffer[nIndex + 3] = 255;
					}
					pBuffer += nAdd; lBufLen -= nAdd;
				}
			}

			*ppDstBuffer = pBgraBuffer;
			*pulWidth    = ulWidth;
			*pulHeight   = ulHeight;

			return false;
		}
		else if (BI_BITCOUNT_5 == ushBitCount)
		{
			if (BI_RGB != unCompression)
				return false; // TODO: Сделать данный вариант, как только будет файлы с данным типом

			// Считываем саму картинку
			int lCalcLen = (((nWidth * ushPlanes * ushBitCount + 31) & ~31) / 8) * abs(nHeight);
			if (lCalcLen != lBufLen)
				return false;

			// 1 байт на каждый канал
			// (Ширина * 3) должна быть кратна 4.
			int nAdd = 0;
			while (0 != div_t(div(3 * nWidth + nAdd, 4)).rem)
			{
				nAdd++;
			}

			int nSize = nWidth * nHeight * 4;

			pBgraBuffer = new BYTE[nWidth * abs(nHeight) * 4 * sizeof(BYTE)];
			if (NULL == pBgraBuffer)
				return false;

			ulHeight = (unsigned short)abs(nHeight);
			ulWidth  = (unsigned short)nWidth;

			if (nHeight < 0)
			{
				for (int nY = 0; nY < abs(nHeight); nY++)
				{
					for (int nX = 0; nX < nWidth; nX++)
					{
						int nIndex = 4 * (nWidth * nY + nX);

						pBgraBuffer[nIndex + 0] = pBuffer[0]; pBuffer++; lBufLen--;
						pBgraBuffer[nIndex + 1] = pBuffer[0]; pBuffer++; lBufLen--;
						pBgraBuffer[nIndex + 2] = pBuffer[0]; pBuffer++; lBufLen--;
						pBgraBuffer[nIndex + 3] = 255;
					}
					pBuffer += nAdd; lBufLen -= nAdd;
				}
			}
			else
			{
				for (int nY = abs(nHeight) - 1; nY >= 0; nY--)
				{
					for (int nX = 0; nX < nWidth; nX++)
					{
						int nIndex = 4 * (nWidth * nY + nX);
						pBgraBuffer[nIndex + 0] = pBuffer[0]; pBuffer++; lBufLen--;
						pBgraBuffer[nIndex + 1] = pBuffer[0]; pBuffer++; lBufLen--;
						pBgraBuffer[nIndex + 2] = pBuffer[0]; pBuffer++; lBufLen--;
						pBgraBuffer[nIndex + 3] = 255;
					}
					pBuffer += nAdd; lBufLen -= nAdd;
				}
			}

			*ppDstBuffer = pBgraBuffer;
			*pulWidth    = ulWidth;
			*pulHeight   = ulHeight;

			return true;
		}
		else if (BI_BITCOUNT_6 == ushBitCount)
		{
			unsigned int ulMaskR  = 0xff000000, ulMaskB = 0x00ff0000, ulMaskG = 0x0000ff00;
			unsigned int ulShiftR = 24, ulShiftB = 16, ulShiftG = 8;
			double dKoefR = 1.0, dKoefB = 1.0, dKoefG = 1.0;
			bool bMask = false;

			if (BI_RGB == unCompression)
			{
				// Маски, сдвиги и коэффициенты уже заполнены стандартными значениями для масок
			}
			else if (BI_BITFIELDS == unCompression)
			{
				if (oHeaderStream.CanRead() < 12)
					return false;

				oHeaderStream >> ulMaskB;
				oHeaderStream >> ulMaskG;
				oHeaderStream >> ulMaskR;

				ulShiftR = GetLowestBit(ulMaskR);
				ulShiftB = GetLowestBit(ulMaskB);
				ulShiftG = GetLowestBit(ulMaskG);

				dKoefR = 255.0 / (ulMaskR >> ulShiftR);
				dKoefG = 255.0 / (ulMaskG >> ulShiftG);
				dKoefB = 255.0 / (ulMaskB >> ulShiftB);

				bMask = true;
			}
			else
				return false;

			// Считываем саму картинку
			int lCalcLen = (((nWidth * ushPlanes * ushBitCount + 31) & ~31) / 8) * abs(nHeight);
			if (lCalcLen != lBufLen)
				return false;

			// 1 байт на каждый канал
			// Ширина должна быть кратна 4.
			int nAdd = 0;
			while (0 != div_t(div(nWidth + nAdd, 4)).rem)
			{
				nAdd++;
			}

			int nSize = nWidth * nHeight * 4;

			pBgraBuffer = new BYTE[(nWidth + nAdd) * abs(nHeight) * 4 * sizeof(BYTE)];
			if (NULL == pBgraBuffer)
				return false;

			ulHeight = (unsigned short)abs(nHeight);
			ulWidth  = (unsigned short)(nWidth + nAdd);

			if (nHeight < 0)
			{
				for (int nY = 0; nY < abs(nHeight); nY++)
				{
					for (int nX = 0; nX < nWidth; nX++)
					{
						int nIndex = 4 * ((nWidth + nAdd) * nY + nX);

						if (bMask)
						{
							unsigned int unValue = ((pBuffer[3] << 24) | (pBuffer[2] << 16) | (pBuffer[1] << 8) | pBuffer[0]) & 0xFFFFFFFF; pBuffer += 4; lBufLen -= 4;

							unsigned char unR = (unValue & ulMaskR) >> ulShiftR;
							unsigned char unG = (unValue & ulMaskG) >> ulShiftG;
							unsigned char unB = (unValue & ulMaskB) >> ulShiftB;

							pBgraBuffer[nIndex + 0] = (unsigned char)(unR * dKoefR);
							pBgraBuffer[nIndex + 1] = (unsigned char)(unG * dKoefG);
							pBgraBuffer[nIndex + 2] = (unsigned char)(unB * dKoefB);
							pBgraBuffer[nIndex + 3] = pBuffer[3];
						}
						else
						{
							pBgraBuffer[nIndex + 0] = pBuffer[0]; pBuffer++; lBufLen--;
							pBgraBuffer[nIndex + 1] = pBuffer[0]; pBuffer++; lBufLen--;
							pBgraBuffer[nIndex + 2] = pBuffer[0]; pBuffer++; lBufLen--;
							pBgraBuffer[nIndex + 3] = 255; pBuffer++; lBufLen--; // Если брать значение из картинки, тогда она получается всегда прозрачной
						}
					}
					for (int nX = nWidth; nX < nWidth + nAdd; nX++)
					{
						int nIndex = 4 * ((nWidth + nAdd) * nY + nX);

						pBgraBuffer[nIndex + 0] = 255;
						pBgraBuffer[nIndex + 1] = 255;
						pBgraBuffer[nIndex + 2] = 255;
						pBgraBuffer[nIndex + 3] = 0;
					}
				}
			}
			else
			{
				for (int nY = abs(nHeight) - 1; nY >= 0; nY--)
				{
					for (int nX = 0; nX < nWidth; nX++)
					{
						int nIndex = 4 * ((nWidth + nAdd) * nY + nX);
						if (bMask)
						{
							unsigned int unValue = ((pBuffer[3] << 24) | (pBuffer[2] << 16) | (pBuffer[1] << 8) | pBuffer[0]) & 0xFFFFFFFF; pBuffer += 4; lBufLen -= 4;

							unsigned char unR = (unValue & ulMaskR) >> ulShiftR;
							unsigned char unG = (unValue & ulMaskG) >> ulShiftG;
							unsigned char unB = (unValue & ulMaskB) >> ulShiftB;

							pBgraBuffer[nIndex + 0] = (unsigned char)(unR * dKoefR);
							pBgraBuffer[nIndex + 1] = (unsigned char)(unG * dKoefG);
							pBgraBuffer[nIndex + 2] = (unsigned char)(unB * dKoefB);
							pBgraBuffer[nIndex + 3] = pBuffer[3];
						}
						else
						{
							pBgraBuffer[nIndex + 0] = pBuffer[0]; pBuffer++; lBufLen--;
							pBgraBuffer[nIndex + 1] = pBuffer[0]; pBuffer++; lBufLen--;
							pBgraBuffer[nIndex + 2] = pBuffer[0]; pBuffer++; lBufLen--;
							pBgraBuffer[nIndex + 3] = 255; pBuffer++; lBufLen--; // Если брать значение из картинки, тогда она получается всегда прозрачной
						}
					}

					for (int nX = nWidth; nX < nWidth + nAdd; nX++)
					{
						int nIndex = 4 * ((nWidth + nAdd) * nY + nX);

						pBgraBuffer[nIndex + 0] = 255;
						pBgraBuffer[nIndex + 1] = 255;
						pBgraBuffer[nIndex + 2] = 255;
						pBgraBuffer[nIndex + 3] = 0;
					}

				}
			}

			*ppDstBuffer = pBgraBuffer;
			*pulWidth    = ulWidth;
			*pulHeight   = ulHeight;

			return true;
		}

		return false;
	}
	void ReadImage(BYTE* pHeaderBuffer, unsigned int ulHeaderBufferLen, BYTE* pImageBuffer, unsigned int ulImageBufferLen, BYTE** ppDstBuffer, unsigned int* pulWidth, unsigned int* pulHeight)
	{
		if (ulHeaderBufferLen <= 0 || NULL == pHeaderBuffer || NULL == pImageBuffer || ulImageBufferLen < 0)
			return;

		CDataStream oHeaderStream;
		oHeaderStream.SetStream(pHeaderBuffer, ulHeaderBufferLen);

		// Считываем заголовок
		unsigned int ulHeaderSize;
		oHeaderStream >> ulHeaderSize;

		if (ulHeaderSize < 0x0000000C)
			return;
		else if (0x0000000C == ulHeaderSize) // BitmapCoreHeader
			ReadImageCoreHeader(pHeaderBuffer + 4, ulHeaderBufferLen - 4, pImageBuffer, ulImageBufferLen, ppDstBuffer, pulWidth, pulHeight);
		else // BitmapInfoHeader
			ReadImageInfoHeader(pHeaderBuffer + 4, ulHeaderBufferLen - 4, pImageBuffer, ulImageBufferLen, ppDstBuffer, pulWidth, pulHeight);
	}
	void ReadImage(BYTE* pImageBuffer, unsigned int unBufferLen, unsigned int unColorUsage, BYTE** ppDstBuffer, unsigned int* punWidth, unsigned int* punHeight)
	{
		if (unBufferLen <= 0 || NULL == pImageBuffer)
			return;

		CDataStream oHeaderStream;
		oHeaderStream.SetStream(pImageBuffer, unBufferLen);

		// Считываем заголовок
		unsigned int unHeaderSize;
		oHeaderStream >> unHeaderSize;

		if (unHeaderSize > unBufferLen)
			return;
		else if (unHeaderSize < 0x0000000C)
			return;
		else if (0x0000000C == unHeaderSize) // BitmapCoreHeader
			ReadImageCoreHeader(pImageBuffer + 4, unHeaderSize - 4, pImageBuffer + unHeaderSize, unBufferLen - unHeaderSize, ppDstBuffer, punWidth, punHeight);
		else // BitmapInfoHeader
		{
			int nWidth;
			int nHeight;

			unsigned short ushPlanes;
			unsigned short ushBitCount;

			unsigned int unCompression;
			unsigned int unImageSize;
			unsigned int unXPelsPerMeter;
			unsigned int unYPelsPerMeter;
			unsigned int unColorUsed;
			unsigned int unColorImportant;

			oHeaderStream >> nWidth;
			oHeaderStream >> nHeight;
			oHeaderStream >> ushPlanes;
			oHeaderStream >> ushBitCount;
			oHeaderStream >> unCompression;
			oHeaderStream >> unImageSize;
			oHeaderStream >> unXPelsPerMeter;
			oHeaderStream >> unYPelsPerMeter;
			oHeaderStream >> unColorUsed;
			oHeaderStream >> unColorImportant;

			if (DIB_RGB_COLORS == unColorUsage)
			{
				int lCalcLen = (((nWidth * ushPlanes * ushBitCount + 31) & ~31) / 8) * abs(nHeight);

				if (unCompression == 0)
				{
					int nAvailableLen = (unBufferLen - unHeaderSize) - lCalcLen - unColorUsed * 4;
					if (nAvailableLen < 0)
						return;

					if (0 == unColorUsed && BI_BITCOUNT_1 == ushBitCount && nAvailableLen >= 8)
						unColorUsed = 2;
					else if (0 == unColorUsed && BI_BITCOUNT_3 == ushBitCount && nAvailableLen >= 1024)
						unColorUsed = 256;
				}

				unHeaderSize += 4 * unColorUsed; // RGBQuad
				ReadImageInfoHeader(pImageBuffer + 4, unHeaderSize - 4, pImageBuffer + unHeaderSize, unBufferLen - unHeaderSize, ppDstBuffer, punWidth, punHeight);
			}
			else
			{
				// TODO: реализовать другие типы цветов
			}
		}
	}
	double GetEllipseAngle(int nL, int nT, int nR, int nB, int nX, int nY)
	{
		double dX0 = (nL + nR) / 2.0;
		double dY0 = (nT + nB) / 2.0;

		// Определим квадрант
		int nQuarter = -1;
		if (nX >= dX0)
		{
			if (nY <= dY0)
				nQuarter = 3;
			else
				nQuarter = 0;
		}
		else
		{
			if (nY <= dY0)
				nQuarter = 2;
			else
				nQuarter = 1;
		}

		double dDist = sqrt((double)(nX - dX0) * (nX - dX0) + (nY - dY0) * (nY - dY0));
		double dRadAngle = asin(fabs(nY - dY0) / dDist);

		double dAngle = dRadAngle * 180 / 3.14159265358979323846;
		switch (nQuarter)
		{
			case 1: dAngle = 180 - dAngle; break;
			case 2: dAngle = 180 + dAngle; break;
			case 3: dAngle = 360 - dAngle; break;
		}

		return dAngle;
	}
	void ProcessRasterOperation(unsigned int unRasterOperation, BYTE** ppBgra, unsigned int unWidth, unsigned int unHeight)
	{
		BYTE* pBgra = *ppBgra;
		// Для битовых операций SRCPAINT и SRCAND сделаем, как будто фон чисто белый.
		if (0x008800C6 == unRasterOperation) // SRCPAINT
		{
			BYTE* pCur = pBgra;
			for (unsigned int unY = 0; unY < unHeight; unY++)
			{
				for (unsigned int unX = 0; unX < unWidth; unX++)
				{
					unsigned int unIndex = (unY * unWidth + unX) * 4;

					if (0xff == pCur[unIndex + 0] && 0xff == pCur[unIndex + 1] && 0xff == pCur[unIndex + 2])
						pCur[unIndex + 3] = 0;
				}
			}
		}
		else if (0x00EE0086 == unRasterOperation) // SRCAND
		{
			BYTE* pCur = pBgra;
			for (unsigned int unY = 0; unY < unHeight; unY++)
			{
				for (unsigned int unX = 0; unX < unWidth; unX++)
				{
					unsigned int unIndex = (unY * unWidth + unX) * 4;

					if (0 == pCur[unIndex + 0] && 0 == pCur[unIndex + 1] && 0 == pCur[unIndex + 2])
						pCur[unIndex + 3] = 0;
				}
			}
		}
	}

	std::wstring ascii_to_unicode(const char *src)
	{
		size_t nSize = mbstowcs(0, src, 0);
		wchar_t* pBuffer = new wchar_t[nSize];
		nSize = mbstowcs(pBuffer, src, nSize);
		std::wstring sRes;
		if (nSize != (size_t)-1)
			sRes = std::wstring(pBuffer, nSize);
		delete[] pBuffer;
		return sRes;
	}
	std::string unicode_to_ascii(const wchar_t *src)
	{
		size_t nSize = wcstombs(0, src, 0);
		char* pBuffer = new char[nSize];
		nSize = wcstombs(pBuffer, src, nSize);
		std::string sRes;
		if (nSize != (size_t)-1)
			sRes = std::string(pBuffer, nSize);
		delete[] pBuffer;
		return sRes;
	}

    bool OpenTempFile(std::wstring *pwsName, FILE **ppFile, const wchar_t *wsMode, const wchar_t *wsExt, const wchar_t *wsFolder)
	{
		std::wstring wsTemp, wsFileName;
		FILE *pTempFile = NULL;
#if defined(_WIN32) || defined (_WIN64)
		wchar_t *wsTempDir;
		if ((wsTempDir = _wgetenv(L"TEMP")) && (wsFolder == NULL))
		{
			wsTemp = std::wstring(wsTempDir);
#else
		char *wsTempDirA;
		if ((wsTempDirA = getenv("TEMP")) && (wsFolder == NULL))
		{
			std::wstring wsTempDir = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)wsTempDirA, strlen(wsTempDirA));
			wsTemp = wsTempDir.c_str();
#endif
			wsTemp.append(L"/");
		}
		else if (wsFolder != NULL)
		{
			wsTemp = std::wstring(wsFolder);
			wsTemp.append(L"/");
		}
		else
		{
			wsTemp = L"";
		}
		wsTemp.append(L"x");
		int nTime = (int)time(NULL);
		for (int nIndex = 0; nIndex < 1000; ++nIndex)
		{
			wsFileName = wsTemp;
#if defined(_WIN32) || defined (_WIN64)
			wchar_t buff[32] ={};
			_itow(nTime + nIndex, buff, 10);
			wsFileName.append(buff, wcslen(buff));
#else
			wsFileName.append(std::to_wstring(nTime + nIndex));
#endif
			if (wsExt)
			{
				wsFileName.append(wsExt);
			}
#if defined (_WIN32) || defined (_WIN64)
			if (!(pTempFile = _wfopen(wsFileName.c_str(), L"r")))
			{
				if (!(pTempFile = _wfopen(wsFileName.c_str(), wsMode)))
#else
			std::string sFileName = U_TO_UTF8(wsFileName);
			if (!(pTempFile = fopen(sFileName.c_str(), "r")))
			{
				std::wstring strMode(wsMode);
				std::string sMode = U_TO_UTF8(strMode);
				if (!(pTempFile = fopen(sFileName.c_str(), sMode.c_str())))
#endif
				{
					return FALSE;
				}
				*pwsName = wsFileName;
				*ppFile = pTempFile;
				return TRUE;
			}

			fclose(pTempFile);
			}

		return FALSE;
		}
		}
