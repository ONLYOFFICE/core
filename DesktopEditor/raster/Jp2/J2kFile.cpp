#include "J2kFile.h"
#include "J2kIncludes.h"
#include "Utils.h"

namespace Jpeg2000
{
	//extern Image* Jp2ToImage(const std::wstring& wsFileName, DecoderParams* pDecoderParams);
	//extern Image* J2kToImage(const std::wstring& wsFileName, DecoderParams* pDecoderParams);
	//extern Image* JptToImage(const std::wstring& wsFileName, DecoderParams* pDecoderParams);
	//extern Image* Mj2ToImage(const std::wstring& wsFileName, DecoderParams* pDecoderParams);
	//extern bool   ImageToJ2k(Image *pImage, const std::wstring& wsFilePath, EncoderParams* pEncoderParams);
	//extern bool   ImageToJ2p(Image *pImage, const std::wstring& wsFilePath, EncoderParams* pEncoderParams);

	//extern void   Image_Destroy(Image *pImage);
	//extern Image* Image_Create(int nComponentsCount, ImageComponentParams *pCompParams, ColorSpace eColorSpace);

	void* Malloc(size_t nSize)
	{
		void *pMemBlock = malloc(nSize);
		if (!pMemBlock)
		{
			// TODO: print error "Memory !!\n")
		}
		else
		{
			memset(pMemBlock, 0x00, nSize);
		}
		return pMemBlock;
	}
	int check_j2000_type(FILE* pFile)
	{
		if (!pFile)
			return 0;

		BYTE *pBuffer = new BYTE[1024];

		if (!pBuffer)
			return 0;

		size_t nBytesRead = fread(pBuffer, 1, 1024, pFile);

		int type =0;
		if ((32 <= nBytesRead) && (0x00 == pBuffer[0] && 0x00 == pBuffer[1] && 0x00 == pBuffer[2] && 0x0c == pBuffer[3]
			&& 0x6a == pBuffer[4] && 0x50 == pBuffer[5] && 0x20 == pBuffer[6] && 0x20 == pBuffer[7]

			&& 0x0d == pBuffer[8] && 0x0a == pBuffer[9] && 0x87 == pBuffer[10] && 0x0a == pBuffer[11]
			&& 0x00 == pBuffer[12] && 0x00 == pBuffer[13] && 0x00 == pBuffer[14] /*&&  (0x14 == pBuffer[15] || 0x18 == pBuffer[15] )*/

			&& 0x66 == pBuffer[16] && 0x74 == pBuffer[17] && 0x79 == pBuffer[18] && 0x70 == pBuffer[19]
			&& 0x6a == pBuffer[20] && 0x70 == pBuffer[21] && 0x32 == pBuffer[22] && 0x20 == pBuffer[23]

			&& 0x00 == pBuffer[24] && 0x00 == pBuffer[25] && 0x00 == pBuffer[26] && 0x00 == pBuffer[27]
			/*&& 0x6a == pBuffer[28] && 0x70 == pBuffer[29]  && 0x32 == pBuffer[30] && 0x20 == pBuffer[31]*/))
		{
			type = 1;
		}
		if ((4 <= nBytesRead) && (0xff == pBuffer[0] && 0x4f == pBuffer[1] && 0xff == pBuffer[2] && 0x51 == pBuffer[3]))
		{
			type = 2;
		}
		if ((32 <= nBytesRead) && (0x00 == pBuffer[0] && 0x00 == pBuffer[1] && 0x00 == pBuffer[2] && 0x0c == pBuffer[3]
			&& 0x6a == pBuffer[4] && 0x50 == pBuffer[5] && 0x20 == pBuffer[6] && 0x20 == pBuffer[7]

			&& 0x0d == pBuffer[8] && 0x0a == pBuffer[9] && 0x87 == pBuffer[10] && 0x0a == pBuffer[11]
			&& 0x00 == pBuffer[12] && 0x00 == pBuffer[13] && 0x00 == pBuffer[14] && 0x18 == pBuffer[15]

			&& 0x66 == pBuffer[16] && 0x74 == pBuffer[17] && 0x79 == pBuffer[18] && 0x70 == pBuffer[19]
			&& 0x6d == pBuffer[20] && 0x6a == pBuffer[21] && 0x70 == pBuffer[22] && 0x32 == pBuffer[23]

			&& 0x00 == pBuffer[24] && 0x00 == pBuffer[25] && 0x00 == pBuffer[26] && 0x00 == pBuffer[27]
			&& 0x6d == pBuffer[28] && 0x6a == pBuffer[29] && 0x70 == pBuffer[30] && 0x32 == pBuffer[31]))
		{
			type = 3;
		}

		delete[] pBuffer;
		fseek(pFile, 0, SEEK_SET);
		return type;
	}

	// CJ2kFile
	bool CJ2kFile::Open(CBgraFrame* pFrame, const std::wstring& wsSrcPath, const std::wstring& wsXmlOptions)
	{
		Image *pImage = NULL;

		DecoderParams oParameters;

		// Установим стандартные значения параметров
		ApplyDecoderOptions(&oParameters, wsXmlOptions);

		///////////////////////////////////////////////////////////////////////////////////

		NSFile::CFileBinary oFile;
		if (!oFile.OpenFile(wsSrcPath))
			return false;

		DWORD nFileSize = oFile.GetFileSize();

		int type = check_j2000_type(oFile.GetFileNative());
		oFile.CloseFile();

		bool bOpenResult = false;
		if (!bOpenResult && type == 1)
			bOpenResult = (NULL != (pImage = Jp2ToImage(wsSrcPath, &oParameters)));

		if (!bOpenResult && type == 2)
			bOpenResult = (NULL != (pImage = J2kToImage(wsSrcPath, &oParameters)));

		if (!bOpenResult && type == 3)
			bOpenResult = (NULL != (pImage = Mj2ToImage(wsSrcPath, &oParameters)));

		if (!bOpenResult && type == 4)
			bOpenResult = (NULL != (pImage = JptToImage(wsSrcPath, &oParameters)));

		if (!bOpenResult)
		{
			Image_Destroy(pImage);
			return false;
		}

		int nWidth  = pImage->pComponents[0].nWidth;
		int nHeight = pImage->pComponents[0].nHeight;
		int nBufferSize = 4 /*pImage->nCsiz*/ * nWidth * nHeight;

		if (nBufferSize < 1)
		{
			Image_Destroy(pImage);
			return false;
		}

		pFrame->put_Width(nWidth);
		pFrame->put_Height(nHeight);
		pFrame->put_Stride(4 * nWidth);

		BYTE* pData = new BYTE[nBufferSize];
		if (!pData)
		{
			Image_Destroy(pImage);
			return false;
		}

		pFrame->put_Data(pData);

		unsigned char* pBufferPtr = (unsigned char*)pData;
		long nCreatedBufferSize = nBufferSize;

		// Пишем данные в pBufferPtr
		if (pImage->nCsiz == 3 && pImage->pComponents[0].nXRsiz == pImage->pComponents[1].nXRsiz && pImage->pComponents[1].nXRsiz == pImage->pComponents[2].nXRsiz
			&& pImage->pComponents[0].nYRsiz == pImage->pComponents[1].nYRsiz && pImage->pComponents[1].nYRsiz == pImage->pComponents[2].nYRsiz
			&& pImage->pComponents[0].nPrecision == pImage->pComponents[1].nPrecision && pImage->pComponents[1].nPrecision == pImage->pComponents[2].nPrecision)
		{
			int nResW = CeilDivPow2(pImage->pComponents[0].nWidth, pImage->pComponents[0].nFactorDiv2);
			int nResH = CeilDivPow2(pImage->pComponents[0].nHeight, pImage->pComponents[0].nFactorDiv2);


			for (int nIndex = 0; nIndex < nResW * nResH; nIndex++)
			{
				unsigned char nR = pImage->pComponents[0].pData[nWidth * nResH - ((nIndex) / (nResW)+1) * nWidth + (nIndex) % (nResW)];
				unsigned char nG = pImage->pComponents[1].pData[nWidth * nResH - ((nIndex) / (nResW)+1) * nWidth + (nIndex) % (nResW)];
				unsigned char nB = pImage->pComponents[2].pData[nWidth * nResH - ((nIndex) / (nResW)+1) * nWidth + (nIndex) % (nResW)];

				pBufferPtr[0] = nB;
				pBufferPtr[1] = nG;
				pBufferPtr[2] = nR;
				pBufferPtr[3] = 255;
				pBufferPtr += 4;

			}
		}
		else if (pImage->nCsiz >= 4 && pImage->pComponents[0].nXRsiz == pImage->pComponents[1].nXRsiz && pImage->pComponents[1].nXRsiz == pImage->pComponents[2].nXRsiz && pImage->pComponents[2].nXRsiz == pImage->pComponents[3].nXRsiz
				 && pImage->pComponents[0].nYRsiz == pImage->pComponents[1].nYRsiz && pImage->pComponents[1].nYRsiz == pImage->pComponents[2].nYRsiz && pImage->pComponents[2].nYRsiz == pImage->pComponents[3].nYRsiz
				 && pImage->pComponents[0].nPrecision == pImage->pComponents[1].nPrecision && pImage->pComponents[1].nPrecision == pImage->pComponents[2].nPrecision && pImage->pComponents[2].nPrecision == pImage->pComponents[3].nPrecision)
		{
			int nResW = CeilDivPow2(pImage->pComponents[0].nWidth, pImage->pComponents[0].nFactorDiv2);
			int nResH = CeilDivPow2(pImage->pComponents[0].nHeight, pImage->pComponents[0].nFactorDiv2);


			for (int nIndex = 0; nIndex < nResW * nResH; nIndex++)
			{
				unsigned char nR = pImage->pComponents[0].pData[nWidth * nResH - ((nIndex) / (nResW)+1) * nWidth + (nIndex) % (nResW)];
				unsigned char nG = pImage->pComponents[1].pData[nWidth * nResH - ((nIndex) / (nResW)+1) * nWidth + (nIndex) % (nResW)];
				unsigned char nB = pImage->pComponents[2].pData[nWidth * nResH - ((nIndex) / (nResW)+1) * nWidth + (nIndex) % (nResW)];
				unsigned char nA = pImage->pComponents[3].pData[nWidth * nResH - ((nIndex) / (nResW)+1) * nWidth + (nIndex) % (nResW)];

				pBufferPtr[0] = nB;
				pBufferPtr[1] = nG;
				pBufferPtr[2] = nR;
				pBufferPtr[3] = nA;
				pBufferPtr += 4;

			}
		}
		else // Grayscale
		{
			int nResW = CeilDivPow2(pImage->pComponents[0].nWidth, pImage->pComponents[0].nFactorDiv2);
			int nResH = CeilDivPow2(pImage->pComponents[0].nHeight, pImage->pComponents[0].nFactorDiv2);

			for (int nIndex = 0; nIndex < nResW * nResH; nIndex++)
			{
				unsigned char nG = pImage->pComponents[0].pData[nWidth * nResH - ((nIndex) / (nResW)+1) * nWidth + (nIndex) % (nResW)];
				pBufferPtr[0] = nG;
				pBufferPtr[1] = nG;
				pBufferPtr[2] = nG;
				pBufferPtr[3] = 255;
				pBufferPtr += 4;
			}
		}

		Image_Destroy(pImage);
		return true;
	}
	bool CJ2kFile::Open(BYTE** ppData, int& nComponentsCount, int& nWidth, int& nHeight, const std::wstring& wsSrcPath, const std::wstring& wsXmlOptions)
	{
		Image *pImage = NULL;

		DecoderParams oParameters;

		// Установим стандартные значения параметров
		ApplyDecoderOptions(&oParameters, wsXmlOptions);

		///////////////////////////////////////////////////////////////////////////////////

		NSFile::CFileBinary oFile;
		if (!oFile.OpenFile(wsSrcPath))
			return false;

		DWORD nFileSize = oFile.GetFileSize();

		int type = check_j2000_type(oFile.GetFileNative());
		oFile.CloseFile();

		bool bOpenResult = false;
		if (!bOpenResult && type == 1)
			bOpenResult = (NULL != (pImage = Jp2ToImage(wsSrcPath, &oParameters)));

		if (!bOpenResult && type == 2)
			bOpenResult = (NULL != (pImage = J2kToImage(wsSrcPath, &oParameters)));

		if (!bOpenResult && type == 3)
			bOpenResult = (NULL != (pImage = Mj2ToImage(wsSrcPath, &oParameters)));

		if (!bOpenResult && type == 4)
			bOpenResult = (NULL != (pImage = JptToImage(wsSrcPath, &oParameters)));

		if (!bOpenResult)
		{
			Image_Destroy(pImage);
			return false;
		}

		nWidth  = pImage->pComponents[0].nWidth;
		nHeight = pImage->pComponents[0].nHeight;

		int nBufferSize = pImage->nCsiz * nWidth * nHeight;
		if (nBufferSize < 1 || pImage->nCsiz <= 0)
		{
			Image_Destroy(pImage);
			return false;
		}

		*ppData = new BYTE[nBufferSize];
		if (!(*ppData))
		{
			Image_Destroy(pImage);
			return false;
		}

		unsigned char* pBufferPtr = (unsigned char*)(*ppData);
		long nCreatedBufferSize = nBufferSize;

		nComponentsCount = pImage->nCsiz;

		// Пишем данные в pBufferPtr
		for (int nComponent = 1; nComponent < nComponentsCount; nComponent++)
		{
			if (pImage->pComponents[0].nXRsiz != pImage->pComponents[nComponent].nXRsiz
				|| pImage->pComponents[0].nYRsiz != pImage->pComponents[nComponent].nYRsiz
				|| pImage->pComponents[0].nPrecision != pImage->pComponents[nComponent].nPrecision)

			{
				delete[](*ppData);
				Image_Destroy(pImage);
				return false;
			}
		}

		int nResW = CeilDivPow2(pImage->pComponents[0].nWidth, pImage->pComponents[0].nFactorDiv2);
		int nResH = CeilDivPow2(pImage->pComponents[0].nHeight, pImage->pComponents[0].nFactorDiv2);

		for (int nIndex = 0; nIndex < nResW * nResH; nIndex++)
		{
			for (int nComponent = 0; nComponent < nComponentsCount; nComponent++)
			{
				pBufferPtr[nComponent] = pImage->pComponents[nComponent].pData[nWidth * nResH - ((nIndex) / (nResW)+1) * nWidth + (nIndex) % (nResW)];
			}
			pBufferPtr += nComponentsCount;
		}

		Image_Destroy(pImage);
		return true;
	}
	bool CJ2kFile::Save(CBgraFrame* pFrame, const std::wstring& wsDstPath, const std::wstring& wsXmlOptions)
	{
		// TODO: Запись не реализована, надо доделать.
		return false;

		if (!pFrame)
			return false;

		LONG lWidth         = pFrame->get_Width();
		LONG lHeight        = pFrame->get_Height();
		BYTE* pSourceBuffer = pFrame->get_Data();
		LONG lBufferSize    = 4 * lWidth * lHeight;

		// Далее обрабатываем Xml с параметрами компрессии
		EncoderParams oParameters;
		int nFormat = ApplyEncoderOptions(&oParameters, wsXmlOptions);

		// TODO: Добавить возможность записи альфа-канала
		ImageComponentParams aComponentParams[3]; // Пока пусть будет максимально три компоненты (RGB)
		Image *pImage = NULL;
		int nComponentsCount = oParameters.nComponentsCount;

		memset(&aComponentParams[0], 0, sizeof(ImageComponentParams));
		for (int nIndex = 0; nIndex < nComponentsCount; nIndex++)
		{
			aComponentParams[nIndex].nPrecision = 8;
			aComponentParams[nIndex].nBPP       = 8;
			aComponentParams[nIndex].nSigned    = 0;
			aComponentParams[nIndex].nXRsiz     = oParameters.nSubSamplingDx;
			aComponentParams[nIndex].nYRsiz     = oParameters.nSubSamplingDy;
			aComponentParams[nIndex].nWidth     = (int)lWidth;
			aComponentParams[nIndex].nHeight    = (int)lHeight;
		}

		// Создаем структуру Image
		pImage = Image_Create(nComponentsCount, &aComponentParams[0], csRGB);
		if (!pImage)
			return false;

		pImage->nXOsiz = oParameters.nImageOffsetX0;
		pImage->nYOsiz = oParameters.nImageOffsetY0;
		pImage->nXsiz  = (!pImage->nXOsiz) ? (lWidth - 1) * oParameters.nSubSamplingDx + 1 : pImage->nXOsiz + (lWidth - 1) * oParameters.nSubSamplingDy + 1;
		pImage->nYsiz  = (!pImage->nYOsiz) ? (lHeight - 1) * oParameters.nSubSamplingDy + 1 : pImage->nYOsiz + (lHeight - 1) * oParameters.nSubSamplingDy + 1;

		if (3 == nComponentsCount)
		{
			int nIndex = 0;
			for (int nY = 0; nY < (int)lHeight; nY++)
			{
				for (int nX = 0; nX < (int)lWidth; nX++, pSourceBuffer += 4)
				{
					pImage->pComponents[0].pData[nIndex] = pSourceBuffer[2];
					pImage->pComponents[1].pData[nIndex] = pSourceBuffer[1];
					pImage->pComponents[2].pData[nIndex] = pSourceBuffer[0];
					nIndex++;
				}
			}
		}
		else if (1 == nComponentsCount)
		{
			int nIndex = 0;
			for (int nY = 0; nY < (int)lHeight; nY++)
			{
				for (int nX = 0; nX < (int)lWidth; nX++, pSourceBuffer += 4)
				{
					pImage->pComponents[0].pData[nIndex] = pSourceBuffer[0];
					nIndex++;
				}
			}
		}
		else
		{
			Image_Destroy(pImage);
			return false;
		}

		bool bRes = false;
		switch (nFormat)
		{
			case 0: bRes = ImageToJ2k(pImage, wsDstPath, &oParameters); break;
			case 1: bRes = ImageToJ2p(pImage, wsDstPath, &oParameters); break;
			case -1:
			default: bRes = false;
		}

		Image_Destroy(pImage);
		return bRes;
	}
}