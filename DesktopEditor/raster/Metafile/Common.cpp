#include "Common.h"

#include "../../raster/ImageFileFormatChecker.h"
#include "../../raster/BgraFrame.h"

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
			if (!WmfOpenTempFile(&wsTempFileName, &pTempFile, L"wb", L".wmf0", NULL))
				return false;

			::fwrite(pBuffer, 1, unImageSize, pTempFile);
			::fclose(pTempFile);

			CBgraFrame oFrame;
			oFrame.OpenFile(wsTempFileName);

			// TODO: Как будут файлы сделать чтение.
            //::_wunlink(wsTempFileName.c_str());
            NSFile::CFileBinary::Remove(wsTempFileName);
			return false;
		}
		else if (BI_BITCOUNT_1 == ushBitCount)
		{
			// Двуцветная картинка, значит палитра состоит из 2-х цветов
			TRgbQuad oColor1, oColor2;
			oHeaderStream >> oColor1 >> oColor2;

			// Считываем саму картинку
			int lCalcLen = (((nWidth * ushPlanes * ushBitCount + 31) & ~31) / 8) * abs(nHeight);
			if (lCalcLen != lBufLen)		
				return false;

			pBgraBuffer = new BYTE[nWidth * nHeight * 4 * sizeof(BYTE)];
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
						if (nX == nWidthBytes - 1)
							nBitCount = nLastBitCount;

						for (int nBitIndex = nBitCount; nBitIndex > 0; nBitIndex /= 2)
						{
							int nBit = (nByte & nBitIndex);
							TRgbQuad* pColor = (nBit ? &oColor2 : &oColor1);
							pBgraBuffer[nIndex + 0] = pColor->b;
							pBgraBuffer[nIndex + 1] = pColor->g;
							pBgraBuffer[nIndex + 2] = pColor->r;
							pBgraBuffer[nIndex + 3] = 255;
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
						if (nX == nWidthBytes - 1)
							nBitCount = nLastBitCount;

						for (int nBitIndex = nBitCount; nBitIndex > 0; nBitIndex /= 2)
						{
							int nBit = (nByte & nBitIndex);
							TRgbQuad* pColor = (nBit ? &oColor2 : &oColor1);
							pBgraBuffer[nIndex + 0] = pColor->b;
							pBgraBuffer[nIndex + 1] = pColor->g;
							pBgraBuffer[nIndex + 2] = pColor->r;
							pBgraBuffer[nIndex + 3] = 255;
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
			if (lBufLen < (nScanLineBytes * nHeight))
				return false;

			pBgraBuffer = new BYTE[nWidth * nHeight * 4 * sizeof(BYTE)];
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

			if (lBufLen < (nWidth + nAdd) * nHeight)
				return false;

			pBgraBuffer = new BYTE[nWidth * nHeight * 4 * sizeof(BYTE)];
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

			pBgraBuffer = new BYTE[nWidth * nHeight * 4 * sizeof(BYTE)];
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
			// Пропускаем таблицу цветов (она не нужна)
			pBuffer += unColorUsed * 4; lBufLen -= unColorUsed * 4;

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

			pBgraBuffer = new BYTE[nWidth * nHeight * 4 * sizeof(BYTE)];
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
			// Пропускаем таблицу цветов (она не нужна)
			pBuffer += unColorUsed * 4; lBufLen -= unColorUsed * 4;

			if (BI_RGB != unCompression)
				return false; // TO DO: Сделать данный вариант, как только будет файлы с данным типом

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

			pBgraBuffer = new BYTE[(nWidth + nAdd) * nHeight * 4 * sizeof(BYTE)];
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

						pBgraBuffer[nIndex + 0] = pBuffer[0]; pBuffer++; lBufLen--;
						pBgraBuffer[nIndex + 1] = pBuffer[0]; pBuffer++; lBufLen--;
						pBgraBuffer[nIndex + 2] = pBuffer[0]; pBuffer++; lBufLen--;
						pBgraBuffer[nIndex + 3] = 255; pBuffer++; lBufLen--; // Если брать значение из картинки, тогда она получается всегда прозрачной
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
						pBgraBuffer[nIndex + 0] = pBuffer[0]; pBuffer++; lBufLen--;
						pBgraBuffer[nIndex + 1] = pBuffer[0]; pBuffer++; lBufLen--;
						pBgraBuffer[nIndex + 2] = pBuffer[0]; pBuffer++; lBufLen--;
						pBgraBuffer[nIndex + 3] = 255; pBuffer++; lBufLen--; // Если брать значение из картинки, тогда она получается всегда прозрачной
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
}
