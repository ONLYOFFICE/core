#include "J2kFile.h"
#include "J2kIncludes.h"
#include "Utils.h"

namespace Jpeg2000
{
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
    int check_j2000_type(BYTE* pBuffer, int nSize)
    {
        if (!pBuffer)
            return 0;

        int type = 0;
        if ((32 <= nSize) && (0x00 == pBuffer[0] && 0x00 == pBuffer[1] && 0x00 == pBuffer[2] && 0x0c == pBuffer[3]
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
        if ((4 <= nSize) && (0xff == pBuffer[0] && 0x4f == pBuffer[1] && 0xff == pBuffer[2] && 0x51 == pBuffer[3]))
        {
            type = 2;
        }
        if ((32 <= nSize) && (0x00 == pBuffer[0] && 0x00 == pBuffer[1] && 0x00 == pBuffer[2] && 0x0c == pBuffer[3]
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
        return type;
    }
	int check_j2000_type(FILE* pFile)
	{
		if (!pFile)
			return 0;

		BYTE *pBuffer = new BYTE[1024];

		if (!pBuffer)
			return 0;

		size_t nBytesRead = fread(pBuffer, 1, 1024, pFile);
        int type = check_j2000_type(pBuffer, nBytesRead);
		delete[] pBuffer;
		fseek(pFile, 0, SEEK_SET);
		return type;
	}	

	// CJ2kFile
    bool ImageToFrame(Image* pImage, CBgraFrame* pFrame, bool isBGRA)
    {
        int nWidth  = pImage->pComponents[0].nWidth;
        int nHeight = pImage->pComponents[0].nHeight;
        int nBufferSize = 4 /*pImage->nCsiz*/ * nWidth * nHeight;

        if (nBufferSize < 1)
            return false;

        pFrame->put_Width(nWidth);
        pFrame->put_Height(nHeight);
        pFrame->put_Stride(4 * nWidth);

        BYTE* pData = new BYTE[nBufferSize];
        if (!pData)
            return false;

        pFrame->put_Data(pData);

        unsigned char* pBufferPtr = (unsigned char*)pData;

        unsigned char indR = isBGRA ? 2 : 0;
        unsigned char indG = 1;
        unsigned char indB = isBGRA ? 0 : 2;
        Jpeg2000::ImageComponent* pComponents = pImage->pComponents;

        // Пишем данные в pBufferPtr
        if (pImage->nCsiz == 3 && pImage->pComponents[0].nXRsiz == pImage->pComponents[1].nXRsiz && pImage->pComponents[1].nXRsiz == pImage->pComponents[2].nXRsiz
            && pImage->pComponents[0].nYRsiz == pImage->pComponents[1].nYRsiz && pImage->pComponents[1].nYRsiz == pImage->pComponents[2].nYRsiz
            && pImage->pComponents[0].nPrecision == pImage->pComponents[1].nPrecision && pImage->pComponents[1].nPrecision == pImage->pComponents[2].nPrecision)
        {
            int nResW = CeilDivPow2(pImage->pComponents[0].nWidth, pImage->pComponents[0].nFactorDiv2);
            int nResH = CeilDivPow2(pImage->pComponents[0].nHeight, pImage->pComponents[0].nFactorDiv2);

            int nValue = nWidth * (nResH - 1);
            for (int j = 0; j < nResH; ++j)
            {
                for (int i = 0; i < nResW; ++i)
                {
                    pBufferPtr[indR] = pComponents[0].pData[nValue + i];
                    pBufferPtr[indG] = pComponents[1].pData[nValue + i];
                    pBufferPtr[indB] = pComponents[2].pData[nValue + i];
                    pBufferPtr[3] = 255;
                    pBufferPtr += 4;
                }
                nValue -= nWidth;
            }
        }
        else if (pImage->nCsiz >= 4 && pImage->pComponents[0].nXRsiz == pImage->pComponents[1].nXRsiz && pImage->pComponents[1].nXRsiz == pImage->pComponents[2].nXRsiz && pImage->pComponents[2].nXRsiz == pImage->pComponents[3].nXRsiz
                 && pImage->pComponents[0].nYRsiz == pImage->pComponents[1].nYRsiz && pImage->pComponents[1].nYRsiz == pImage->pComponents[2].nYRsiz && pImage->pComponents[2].nYRsiz == pImage->pComponents[3].nYRsiz
                 && pImage->pComponents[0].nPrecision == pImage->pComponents[1].nPrecision && pImage->pComponents[1].nPrecision == pImage->pComponents[2].nPrecision && pImage->pComponents[2].nPrecision == pImage->pComponents[3].nPrecision)
        {
            int nResW = CeilDivPow2(pImage->pComponents[0].nWidth, pImage->pComponents[0].nFactorDiv2);
            int nResH = CeilDivPow2(pImage->pComponents[0].nHeight, pImage->pComponents[0].nFactorDiv2);

            int nValue = nWidth * (nResH - 1);
            for (int j = 0; j < nResH; ++j)
            {
                for (int i = 0; i < nResW; ++i)
                {
                    pBufferPtr[indR] = pComponents[0].pData[nValue + i];
                    pBufferPtr[indG] = pComponents[1].pData[nValue + i];
                    pBufferPtr[indB] = pComponents[2].pData[nValue + i];
                    pBufferPtr[3]    = pComponents[3].pData[nValue + i];
                    pBufferPtr += 4;
                }
                nValue -= nWidth;
            }
        }
        else // Grayscale
        {
            int nResW = CeilDivPow2(pImage->pComponents[0].nWidth, pImage->pComponents[0].nFactorDiv2);
            int nResH = CeilDivPow2(pImage->pComponents[0].nHeight, pImage->pComponents[0].nFactorDiv2);

            int nValue = nWidth * (nResH - 1);
            for (int j = 0; j < nResH; ++j)
            {
                for (int i = 0; i < nResW; ++i)
                {
                    pBufferPtr[0] = pComponents[0].pData[nValue + i];
                    pBufferPtr[1] = pBufferPtr[0];
                    pBufferPtr[2] = pBufferPtr[0];
                    pBufferPtr[3] = 255;
                    pBufferPtr += 4;
                }
                nValue -= nWidth;
            }
        }

        return true;
    }

	bool CJ2kFile::Open(CBgraFrame* pFrame, const std::wstring& wsSrcPath, const std::wstring& wsXmlOptions, bool isBGRA)
	{
		Image *pImage = NULL;

		DecoderParams oParameters;

		// Установим стандартные значения параметров
		ApplyDecoderOptions(&oParameters, wsXmlOptions);

		///////////////////////////////////////////////////////////////////////////////////

		NSFile::CFileBinary oFile;
		if (!oFile.OpenFile(wsSrcPath))
			return false;

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

        bool bFrame = ImageToFrame(pImage, pFrame, isBGRA);

        Image_Destroy(pImage);
        return bFrame;
	}
	bool CJ2kFile::Open(CBgraFrame* pFrame, BYTE* pBuffer, int nSize,      const std::wstring& wsXmlOptions, bool isBGRA)
	{
		Image *pImage = NULL;

		DecoderParams oParameters;

		// Установим стандартные значения параметров
		ApplyDecoderOptions(&oParameters, wsXmlOptions);

		///////////////////////////////////////////////////////////////////////////////////

		int type = check_j2000_type(pBuffer, nSize);

		bool bOpenResult = false;
		if (!bOpenResult && type == 1)
			bOpenResult = (NULL != (pImage = Jp2ToImage(pBuffer, nSize, &oParameters)));

		if (!bOpenResult && type == 2)
			bOpenResult = (NULL != (pImage = J2kToImage(pBuffer, nSize, &oParameters)));

		if (!bOpenResult && type == 3)
			bOpenResult = (NULL != (pImage = Mj2ToImage(pBuffer, nSize, &oParameters)));

		if (!bOpenResult && type == 4)
			bOpenResult = (NULL != (pImage = JptToImage(pBuffer, nSize, &oParameters)));

		if (!bOpenResult)
		{
			Image_Destroy(pImage);
			return false;
		}

        bool bFrame = ImageToFrame(pImage, pFrame, isBGRA);

        Image_Destroy(pImage);
        return bFrame;
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

	void CJ2kFile::ApplyDecoderOptions(void *pParametersV, const std::wstring& sXml)
	{
		DecoderParams* pParameters = (DecoderParams*)pParametersV;
		memset(pParameters, 0, sizeof(DecoderParams));
		pParameters->eLimitDecoding = declimNoLimitation;
		pParameters->nReduce        = 0;
		pParameters->nLayer         = 0;

		// TODO: Сделать чтение параметров декодирования
		//if (sXml.GetLength() > 0)
		//{
		//	XmlUtils::CXmlNode oMainNode;
		//	oMainNode.FromXmlString(sXml);

		//	if (-1 != oMainNode.GetName().Find(_T("Jpeg2000-Options")))
		//	{
		//		XmlUtils::CXmlNode oOpenNode;
		//		if (oMainNode.GetNode(_T("OpenOptions"), oOpenNode))
		//		{
		//			XmlUtils::CXmlNode oCurNode;
		//			CString sValue;

		//			// ReduceFactor
		//			if (oOpenNode.GetNode(_T("ConvertToRGBA"), oCurNode))
		//			{
		//				sValue = oCurNode.GetAttribute(_T("value"), _T("0"));
		//				pParameters->bConvertToRGBA = (_wtoi(sValue.GetBuffer()) ? true : false);
		//			}

		//			// ReduceFactor
		//			if (oOpenNode.GetNode(_T("ReduceFactor"), oCurNode))
		//			{
		//				sValue = oCurNode.GetAttribute(_T("value"), _T("0"));
		//				pParameters->nReduce = _wtoi(sValue.GetBuffer());
		//			}

		//			// LayersCount
		//			if (oOpenNode.GetNode(_T("LayersCount"), oCurNode))
		//			{
		//				sValue = oCurNode.GetAttribute(_T("value"), _T("0"));
		//				pParameters->nLayer = _wtoi(sValue.GetBuffer());
		//			}
		//		}
		//	}
		//}
	}
	long CJ2kFile::ApplyEncoderOptions(void *pParametersV, const std::wstring& sXml)
	{
		EncoderParams* pParameters = (EncoderParams*)pParameters;
		int nFormat = 0; // J2k
		// Сначала выставляем стандартные значения параметров
		memset(pParameters, 0, sizeof(EncoderParams));
		pParameters->nComponentsCount     = 3;
		pParameters->nResolutionsCount    = 6;
		pParameters->nCodeBlockHeightInit = 64;
		pParameters->nCodeBlockWidthInit  = 64;
		pParameters->eProgOrder           = poLRCP;
		pParameters->nROIComponentIndex   = -1;     // Нет ROI
		pParameters->nSubSamplingDx       = 1;
		pParameters->nSubSamplingDy       = 1;
		pParameters->bTileSizeOn          = false;
		pParameters->sComment             = (char*)"Manufactured by Online Media Technologies Ltd.";

		// TODO: Сделать чтение параметров кодирования
		//if (sXml.GetLength() > 0)
		//{
		//	XmlUtils::CXmlNode oMainNode;
		//	oMainNode.FromXmlString(sXml);

		//	if (-1 != oMainNode.GetName().Find(_T("Jpeg2000-Options")))
		//	{
		//		XmlUtils::CXmlNode oSaveNode;
		//		if (oMainNode.GetNode(_T("SaveOptions"), oSaveNode))
		//		{
		//			XmlUtils::CXmlNode oCurNode;
		//			CString sValue;

		//			// ComponentsCount
		//			if (oSaveNode.GetNode(_T("ComponentsCount"), oCurNode))
		//			{
		//				pParameters->nComponentsCount = XmlUtils::GetInteger(oCurNode.GetAttribute(_T("value"), _T("3")));
		//			}


		//			// Format
		//			if (oSaveNode.GetNode(_T("Format"), oCurNode))
		//			{
		//				sValue = oCurNode.GetAttribute(_T("value"), _T("0"));
		//				nFormat = _wtoi(sValue.GetBuffer());
		//			}

		//			// Tile
		//			if (oSaveNode.GetNode(_T("Tile"), oCurNode))
		//			{
		//				sValue = oCurNode.GetAttribute(_T("width"), _T("32"));
		//				pParameters->nXTsiz = max(32, _wtoi(sValue.GetBuffer()));
		//				sValue = oCurNode.GetAttribute(_T("height"), _T("32"));
		//				pParameters->nYTsiz = max(32, _wtoi(sValue.GetBuffer()));

		//				pParameters->bTileSizeOn = true;

		//				sValue = oCurNode.GetAttribute(_T("offsetx"), _T("0"));
		//				pParameters->nXTOsiz = _wtoi(sValue.GetBuffer());
		//				sValue = oCurNode.GetAttribute(_T("offsety"), _T("0"));
		//				pParameters->nYTOsiz = _wtoi(sValue.GetBuffer());
		//			}

		//			// Resolution
		//			if (oSaveNode.GetNode(_T("Resolution"), oCurNode))
		//			{
		//				sValue = oCurNode.GetAttribute(_T("levelscount"), _T("6"));
		//				pParameters->nResolutionsCount = max(1, min(J2K_MAXRLVLS, _wtoi(sValue.GetBuffer())));
		//			}

		//			// PrecinctDimension
		//			if (oSaveNode.GetNode(_T("PrecinctDimension"), oCurNode))
		//			{
		//				sValue = oCurNode.GetText();
		//				wchar_t *wsTemp = sValue.GetBuffer();

		//				char nSeparateChar = 0;
		//				int nPrecinctsCount = 0;
		//				do
		//				{
		//					nSeparateChar = 0;
		//					swscanf(wsTemp, _T("[%d,%d]%c"), &pParameters->anPrecinctWidthInit[nPrecinctsCount], &pParameters->anPrecinctHeightInit[nPrecinctsCount], &nSeparateChar);
		//					pParameters->nCodingStyle |= 0x01;
		//					nPrecinctsCount++;
		//					if (J2K_MAXRLVLS == nPrecinctsCount)
		//						break;
		//					wsTemp = wcspbrk(wsTemp, _T("]")) + 2;
		//				} while (',' == nSeparateChar);
		//				pParameters->nPrecinctCount = nPrecinctsCount;
		//			}

		//			// CodeBlockDimension
		//			if (oSaveNode.GetNode(_T("CodeBlockDimension"), oCurNode))
		//			{
		//				sValue = oCurNode.GetAttribute(_T("width"), _T("64"));
		//				pParameters->nCodeBlockWidthInit = max(4, min(4096, _wtoi(sValue.GetBuffer())));

		//				sValue = oCurNode.GetAttribute(_T("height"), _T("64"));
		//				pParameters->nCodeBlockHeightInit = max(4, min(4096, _wtoi(sValue.GetBuffer())));
		//			}

		//			// ProgressionOrder
		//			if (oSaveNode.GetNode(_T("ProgressionOrder"), oCurNode))
		//			{
		//				sValue = oCurNode.GetAttribute(_T("value"), _T("0"));
		//				int nProgOrder = max(0, min(4, _wtoi(sValue.GetBuffer())));
		//				switch (nProgOrder)
		//				{
		//					case 0: pParameters->eProgOrder = poLRCP; break;
		//					case 1: pParameters->eProgOrder = poRLCP; break;
		//					case 2: pParameters->eProgOrder = poRPCL; break;
		//					case 3: pParameters->eProgOrder = poPCRL; break;
		//					case 4: pParameters->eProgOrder = poCPRL; break;
		//				}
		//			}

		//			// Subsampling
		//			if (oSaveNode.GetNode(_T("Subsampling"), oCurNode))
		//			{
		//				sValue = oCurNode.GetAttribute(_T("dx"), _T("1"));
		//				pParameters->nSubSamplingDx = max(1, min(J2K_MAXSSFTR, _wtoi(sValue.GetBuffer())));

		//				sValue = oCurNode.GetAttribute(_T("dy"), _T("1"));
		//				pParameters->nSubSamplingDy = max(1, min(J2K_MAXSSFTR, _wtoi(sValue.GetBuffer())));
		//			}

		//			// SOPmarker
		//			if (oSaveNode.GetNode(_T("SOPmarker"), oCurNode))
		//			{
		//				// Пока отключим, потом надо будет добавить - для этого нужно выделять память для стрктуры ImageInfo
		//				//pParameters->nCodingStyle |= 0x02;
		//			}

		//			// EPHmarker
		//			if (oSaveNode.GetNode(_T("EPHmarker"), oCurNode))
		//			{
		//				pParameters->nCodingStyle |= 0x04;
		//			}

		//			// ROI
		//			if (oSaveNode.GetNode(_T("ROI"), oCurNode))
		//			{
		//				sValue = oCurNode.GetAttribute(_T("componentindex"), _T("-1"));
		//				pParameters->nROIComponentIndex = max(-1, _wtoi(sValue.GetBuffer()));

		//				sValue = oCurNode.GetAttribute(_T("shift"), _T("0"));
		//				pParameters->nROIShift = max(0, min(37, _wtoi(sValue.GetBuffer())));
		//			}

		//			// Comment
		//			if (oSaveNode.GetNode(_T("Comment"), oCurNode))
		//			{
		//				sValue = oCurNode.GetAttribute(_T("value"));
		//				// TO DO: Неправильное копирование строки
		//				USES_CONVERSION;
		//				pParameters->sComment = W2A(sValue.GetBuffer());
		//			}

		//			// Compression
		//			if (oSaveNode.GetNode(_T("Compression"), oCurNode))
		//			{
		//				sValue = oCurNode.GetAttribute(_T("lossless"), _T("1"));
		//				pParameters->nTransformId = _wtoi(sValue.GetBuffer()) ? 0 : 1;
		//			}

		//			// Rate-Disto
		//			if (oSaveNode.GetNode(_T("Rate-Disto"), oCurNode))
		//			{
		//				sValue = oCurNode.GetText();

		//				wchar_t *wsTemp = sValue.GetBuffer();

		//				while (swscanf(wsTemp, _T("%f"), &pParameters->afRates[pParameters->nLayersCount]) == 1)
		//				{
		//					pParameters->nLayersCount++;
		//					while (*wsTemp && *wsTemp != _T(','))
		//					{
		//						wsTemp++;
		//					}
		//					if (!*wsTemp)
		//						break;
		//					wsTemp++;
		//				}
		//				pParameters->nDistoAlloc = 1;
		//			}

		//			// Fixed-Quality
		//			if (oSaveNode.GetNode(_T("Fixed-Quality"), oCurNode))
		//			{
		//				sValue = oCurNode.GetText();

		//				wchar_t *wsTemp = sValue.GetBuffer();

		//				while (swscanf(wsTemp, _T("%f"), &pParameters->afDistoratio[pParameters->nLayersCount]) == 1)
		//				{
		//					pParameters->nLayersCount++;
		//					while (*wsTemp && *wsTemp != ',')
		//					{
		//						wsTemp++;
		//					}
		//					if (!*wsTemp)
		//						break;
		//					wsTemp++;
		//				}
		//				pParameters->nFixedQuality = 1;
		//			}

		//			// Fixed-Layer
		//			if (oSaveNode.GetNode(_T("Fixed-Layer"), oCurNode))
		//			{
		//				sValue = oCurNode.GetAttribute(_T("layerscount"), _T("1"));
		//				int nLayersCount = max(1, _wtoi(sValue.GetBuffer()));
		//				pParameters->nLayersCount = nLayersCount;

		//				int nResolutionsCount = pParameters->nResolutionsCount;
		//				int nMatrixWidth = nResolutionsCount * 3;
		//				pParameters->pMatrix = (int*)Malloc(0, nLayersCount * nMatrixWidth * sizeof(int));

		//				sValue = oCurNode.GetText();
		//				wchar_t *wsTemp = sValue.GetBuffer();

		//				for (int nLayerIndex = 0; nLayerIndex < nLayersCount; nLayerIndex++)
		//				{
		//					int *pRow = &pParameters->pMatrix[nLayerIndex * nMatrixWidth];
		//					int *pCol = pRow;

		//					pParameters->afRates[nLayerIndex] = 1;

		//					swscanf(wsTemp, _T("%d,"), &pCol[0]);
		//					wsTemp += 2;

		//					if (pCol[0] > 9)
		//						wsTemp++;

		//					pCol[1] = 0;
		//					pCol[2] = 0;

		//					for (int nResolutionIndex = 1; nResolutionIndex < nResolutionsCount; nResolutionIndex++)
		//					{
		//						pCol += 3;
		//						swscanf(wsTemp, _T("%d,%d,%d"), &pCol[0], &pCol[1], &pCol[2]);
		//						wsTemp += 6;
		//						if (pCol[0] > 9)
		//							wsTemp++;
		//						if (pCol[1] > 9)
		//							wsTemp++;
		//						if (pCol[2] > 9)
		//							wsTemp++;
		//					}
		//					if (nLayerIndex < nLayersCount - 1)
		//						wsTemp++;
		//				}

		//				pParameters->nFixedAlloc = 1;

		//			}
		//		}

		//	}
		//}


		// Проверим кооректность введенных параметров

		// Параметры nDistoAlloc, nFixedQuality, nFixedAlloc нельзя использовать вместе
		if ((pParameters->nDistoAlloc || pParameters->nFixedAlloc || pParameters->nFixedQuality) && (!(pParameters->nDistoAlloc ^ pParameters->nFixedAlloc ^ pParameters->nFixedQuality)))
			return -1;

		// Если параметры не заданы, тогда по умолчанию ставим компрессию без потерь
		if (0 == pParameters->nLayersCount)
		{
			pParameters->afRates[0]   = 0;
			pParameters->nLayersCount = 1;
			pParameters->nDistoAlloc  = 1;
		}
		if (pParameters->nXTOsiz > pParameters->nImageOffsetX0 || pParameters->nYTOsiz > pParameters->nImageOffsetY0)
			return -1;

		for (int nIndex = 0; nIndex < pParameters->nPOCsCount; nIndex++)
		{
			if (-1 == pParameters->aoPOC[nIndex].ePpoc)
			{
				// TO DO: Выдать ошибку, что порядок не задан
			}
		}

		return nFormat;
	}
}
