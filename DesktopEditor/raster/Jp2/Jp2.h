#pragma once
//-------------------------------------------------------------------------------------------------------------------------------

#include "Types.h"
#include "Reader.h"
#include "J2k.h"

namespace Jpeg2000
{
	//-------------------------------------------------------------------------------------------------------------------------------
	// Вспомогательные функции
	//-------------------------------------------------------------------------------------------------------------------------------
	static bool Jp2_ReadBoxHeader(PCommon pCodecInfo, CReader *pStream, Jp2Box *pBox)
	{
		pBox->nInitPos =  pStream->Tell();
		pBox->nLength  = pStream->Read(4);
		pBox->nType    = pStream->Read(4);

		if (pBox->nLength == 1)
		{
			if (pStream->Read(4) != 0)
			{
				Event_Message(EVT_ERROR, "Cannot handle box sizes higher than 2^32\n");
				return false;
			}
			pBox->nLength = pStream->Read(4);
			if (pBox->nLength == 0)
				pBox->nLength = pStream->GetLeftSize() + 12;
		}
		else if (pBox->nLength == 0)
		{
			int nBytesLeft = pStream->GetLeftSize();
			if (0 >= nBytesLeft)
				return false;

			pBox->nLength = nBytesLeft + 8;
		}

		return true;
	}

	static void Jp2_WriteURL(CReader *pStream, char *sIndexFile)
	{
		Jp2Box oBox;

		oBox.nInitPos =  pStream->Tell();
		pStream->Skip(4);
		pStream->Write(JP2_URL, 4); // DBTL
		pStream->Write(0, 1);       // VERS
		pStream->Write(0, 3);       // FLAG

		if (sIndexFile)
		{
			for (unsigned int nIndex = 0; nIndex < strlen(sIndexFile); nIndex++)
			{
				pStream->Write(sIndexFile[nIndex], 1);
			}
		}

		oBox.nLength =  pStream->Tell() - oBox.nInitPos;
		pStream->Seek(oBox.nInitPos);
		pStream->Write(oBox.nLength, 4); // L
		pStream->Seek(oBox.nInitPos + oBox.nLength);
	}

	static bool Jp2_ReadIHDR(Jp2Stream *pJp2, CReader *pStream)
	{
		Jp2Box box;

		PCommon pInfo = pJp2->pCodecInfo;

		Jp2_ReadBoxHeader(pInfo, pStream, &box);
		if (JP2_IHDR != box.nType)
		{
			Event_Message(EVT_ERROR, "Expected IHDR Marker\n");
			return false;
		}

		pJp2->nHeight          = pStream->Read(4); // HEIGHT
		pJp2->nWidth           = pStream->Read(4); // WIDTH
		pJp2->nComponentsCount = pStream->Read(2);	// NC
		pJp2->pComponents      = (Jp2Component*)Malloc(pJp2->nComponentsCount * sizeof(Jp2Component));
		if (!pJp2->pComponents)
		{
			Event_Message(EVT_ERROR, "Not enough memory\n");
			pJp2->pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
			return false;
		}
		pJp2->nBPC             = pStream->Read(1); // BPC
		pJp2->nCompressionType = pStream->Read(1); // C
		pJp2->nColorSpaceUnk   = pStream->Read(1); // UnkC
		pJp2->nIPR             = pStream->Read(1); // IPR

		if (pStream->Tell() - box.nInitPos != box.nLength)
		{
			Event_Message(EVT_ERROR, "Error with IHDR Box\n");
			return false;
		}

		return true;
	}

	static void Jp2_WriteIHDR(Jp2Stream *pJp2, CReader *pStream)
	{
		Jp2Box box;

		box.nInitPos =  pStream->Tell();
		pStream->Skip(4);
		pStream->Write(JP2_IHDR, 4); // IHDR
		pStream->Write(pJp2->nHeight, 4); // HEIGHT
		pStream->Write(pJp2->nWidth, 4); // WIDTH
		pStream->Write(pJp2->nComponentsCount, 2); // NC
		pStream->Write(pJp2->nBPC, 1); // BPC
		pStream->Write(pJp2->nCompressionType, 1); // C ( это значение всегда равно 7 )
		pStream->Write(pJp2->nColorSpaceUnk, 1); // UnkC, colorspace unknown
		pStream->Write(pJp2->nIPR, 1); // IPR

		box.nLength =  pStream->Tell() - box.nInitPos;
		pStream->Seek(box.nInitPos);
		pStream->Write(box.nLength, 4); // L
		pStream->Seek(box.nInitPos + box.nLength);
	}

	static void Jp2_WriteBPCC(Jp2Stream *pJp2, CReader *pStream)
	{
		Jp2Box oBox;
		oBox.nInitPos =  pStream->Tell();
		pStream->Skip(4);
		pStream->Write(JP2_BPCC, 4); // BPCC

		for (unsigned int nIndex = 0; nIndex < pJp2->nComponentsCount; nIndex++)
		{
			pStream->Write(pJp2->pComponents[nIndex].nBPCC, 1);
		}

		oBox.nLength =  pStream->Tell() - oBox.nInitPos;
		pStream->Seek(oBox.nInitPos);
		pStream->Write(oBox.nLength, 4); // L
		pStream->Seek(oBox.nInitPos + oBox.nLength);
	}


	static bool Jp2_ReadBPCC(Jp2Stream *pJp2, CReader *pStream)
	{
		PCommon pCodecInfo = pJp2->pCodecInfo;
		Jp2Box oBox;
		Jp2_ReadBoxHeader(pCodecInfo, pStream, &oBox);
		if (JP2_BPCC != oBox.nType)
		{
			Event_Message(EVT_ERROR, "Expected BPCC Marker\n");
			return false;
		}

		for (unsigned int nIndex = 0; nIndex < pJp2->nComponentsCount; nIndex++)
		{
			pJp2->pComponents[nIndex].nBPCC = pStream->Read(1);
		}

		if (pStream->Tell() - oBox.nInitPos != oBox.nLength)
		{
			Event_Message(EVT_ERROR, "Error with BPCC Box\n");
			return false;
		}

		return true;
	}

	static void Jp2_WriteCOLR(Jp2Stream *pJp2, CReader *pStream)
	{
		Jp2Box oBox;
		oBox.nInitPos =  pStream->Tell();
		pStream->Skip(4);
		pStream->Write(JP2_COLR, 4); // COLR
		//pStream->Write( 1/*pJp2->nMethod*/, 1 ); // METH
		//pStream->Write( 0/*pJp2->nPrec*/,   1 ); // PREC
		//pStream->Write( 0/*pJp2->nApprox*/, 1 ); // APPROX
		//pStream->Write( 16/*pJp2->nEnumCS*/, 4); // EnumCS
		pStream->Write(pJp2->nMethod, 1); // METH
		pStream->Write(pJp2->nPrec, 1); // PREC
		pStream->Write(pJp2->nApprox, 1); // APPROX

		if (1 == pJp2->nMethod)
		{
			pStream->Write(pJp2->nEnumCS, 4); // EnumCS
		}
		else
		{
			pStream->Write(0, 1); // PROFILE
		}

		oBox.nLength =  pStream->Tell() - oBox.nInitPos;
		pStream->Seek(oBox.nInitPos);
		pStream->Write(oBox.nLength, 4); // L
		pStream->Seek(oBox.nInitPos + oBox.nLength);
	}

	static bool Jp2_ReadCOLR(Jp2Stream *pJp2, CReader *pStream)
	{
		if (pJp2->oColor.has_colr)
		{
			// TODO: print error "A conforming JP2 reader shall ignore all Colour Specification boxes after the first, so we ignore this one.\n"
			return true;
		}

		PCommon pCodecInfo = pJp2->pCodecInfo;
		Jp2Box oBox;
		Jp2_ReadBoxHeader(pCodecInfo, pStream, &oBox);

		do
		{
			if (JP2_COLR != oBox.nType)
			{
				if (oBox.nType == JP2_JP2C)
				{
					Event_Message(EVT_ERROR, "Expected JP2H Marker\n");
					return false;
				}
				pStream->Skip(oBox.nLength - 8);
				Jp2_ReadBoxHeader(pCodecInfo, pStream, &oBox);
			}
		} while (JP2_COLR != oBox.nType);

		pJp2->nMethod = pStream->Read(1); // METH
		pJp2->nPrec   = pStream->Read(1); // PREC
		pJp2->nApprox = pStream->Read(1); // APPROX

		if (pJp2->nMethod == 1)
		{
			pJp2->nEnumCS = pStream->Read(4); // EnumCS

			pJp2->oColor.has_colr = true;
		}
		else if (pJp2->nMethod == 2) 
		{
			/* ICC profile */
			int it_icc_value = 0;
			int icc_len = oBox.nLength - 3;

			pJp2->oColor.icc_profile_len = icc_len;
			pJp2->oColor.icc_profile_buf = (BYTE*)Malloc(icc_len);

			if (!pJp2->oColor.icc_profile_buf)
			{
				pJp2->oColor.icc_profile_len = 0;
				return false;
			}

			for (it_icc_value = 0; it_icc_value < icc_len; ++it_icc_value)
			{
				pJp2->oColor.icc_profile_buf[it_icc_value] = (BYTE)pStream->Read(1);		/* icc values */
			}

			pJp2->oColor.has_colr = true;
		}
		else// if (jp2->meth > 2)
		{
			/*	ISO/IEC 15444-1:2004 (E), Table I.9 ­ Legal METH values:
			conforming JP2 reader shall ignore the entire Colour Specification box.*/
			//"COLR BOX meth value is not a regular value (%d), so we will ignore the entire Colour Specification box. \n", jp2->meth);
			// Пропускаем PROFILE
			int nSkipLen = oBox.nInitPos + oBox.nLength - pStream->Tell();
			if (nSkipLen < 0)
			{
				Event_Message(EVT_ERROR, "Error with JP2H box size\n");
				return false;
			}
			pStream->Skip(oBox.nInitPos + oBox.nLength - pStream->Tell());
		}


		if (pStream->Tell() - oBox.nInitPos != oBox.nLength)
		{
			Event_Message(EVT_ERROR, "Error with BPCC Box\n");
			return false;
		}
		return true;
	}

	static void Jp2_WriteCDEF(Jp2Stream *pJp2, CReader *pStream)
	{
	}
	static bool Jp2_ReadCDEF(Jp2Stream *pJp2, CReader *pStream)
	{
		Jp2Box oBox;
		PCommon pCodecInfo = pJp2->pCodecInfo;
		Jp2_ReadBoxHeader(pCodecInfo, pStream, &oBox);
		do
		{
			if (JP2_CDEF != oBox.nType)
			{
				if (0 == oBox.nType)
					return false;
				pStream->Skip(oBox.nLength - 8);
				Jp2_ReadBoxHeader(pCodecInfo, pStream, &oBox);
			}
		} while (JP2_CDEF != oBox.nType);

		int nComponentsCount = pStream->Read(2);

		for (int nIndex = 0; nIndex < nComponentsCount; nIndex++)
		{
			int nCompNum = pStream->Read(2);
			int nType    = pStream->Read(2);
			int nAsoc    = pStream->Read(2);
		}

		return true;
	}
	static void Jp2_WriteJP2H(Jp2Stream *pJp2, CReader *pStream)
	{
		Jp2Box oBox;
		oBox.nInitPos =  pStream->Tell();
		pStream->Skip(4);
		pStream->Write(JP2_JP2H, 4); // JP2H
		Jp2_WriteIHDR(pJp2, pStream);

		if (pJp2->nBPC == 255)
		{
			Jp2_WriteBPCC(pJp2, pStream);
		}
		Jp2_WriteCOLR(pJp2, pStream);

		oBox.nLength =  pStream->Tell() - oBox.nInitPos;
		pStream->Seek(oBox.nInitPos);
		pStream->Write(oBox.nLength, 4); // L
		pStream->Seek(oBox.nInitPos + oBox.nLength);
	}

	static bool Jp2_ReadJP2H(Jp2Stream *pJp2, CReader *pStream)
	{
		Jp2Box oBox;
		PCommon pCodecInfo = pJp2->pCodecInfo;
		Jp2_ReadBoxHeader(pCodecInfo, pStream, &oBox);
		do
		{
			if (JP2_JP2H != oBox.nType)
			{
				if (oBox.nType == JP2_JP2C)
				{
					Event_Message(EVT_ERROR, "Expected JP2H Marker\n");
					return false;
				}
				pStream->Skip(oBox.nLength - 8);
				if (!Jp2_ReadBoxHeader(pCodecInfo, pStream, &oBox))
					return false;
			}
		} while (JP2_JP2H != oBox.nType);

		if (!Jp2_ReadIHDR(pJp2, pStream))
			return false;

		int nCurPos =  pStream->Tell();
		Jp2_ReadCDEF(pJp2, pStream);
		pStream->Seek(nCurPos);

		if (pJp2->nBPC == 255)
		{
			if (!Jp2_ReadBPCC(pJp2, pStream))
				return false;
		}

		nCurPos =  pStream->Tell();
		if (!Jp2_ReadCOLR(pJp2, pStream))
		{
			// return false;

			// По спецификации данный Box является необходимым, но мы все-таки
			// попробуем прочитать изображение со стандартными параметрами.

			pStream->Seek(nCurPos);
			pJp2->nApprox = 0;
			pJp2->nPrec   = 0;

			pJp2->nMethod = 1;
			pJp2->nEnumCS = 16; // sRGB
		}

		int nSkipLen = oBox.nInitPos + oBox.nLength - pStream->Tell();
		if (nSkipLen < 0)
		{
			Event_Message(EVT_ERROR, "Error with JP2H Box\n");
			return false;
		}
		pStream->Skip(oBox.nInitPos + oBox.nLength - pStream->Tell());

		return true;
	}

	static void Jp2_WriteFTYP(Jp2Stream *pJp2, CReader *pStream)
	{
		Jp2Box oBox;
		oBox.nInitPos =  pStream->Tell();
		pStream->Skip(4);
		pStream->Write(JP2_FTYP, 4); // FTYP
		pStream->Write(pJp2->nBrand, 4); // BR
		pStream->Write(pJp2->nMinVersion, 4); // MinV

		for (unsigned int nIndex = 0; nIndex < pJp2->nCompListLength; nIndex++)
		{
			pStream->Write(pJp2->pCompList[nIndex], 4); // CL
		}

		oBox.nLength =  pStream->Tell() - oBox.nInitPos;
		pStream->Seek(oBox.nInitPos);
		pStream->Write(oBox.nLength, 4); // L
		pStream->Seek(oBox.nInitPos + oBox.nLength);
	}

	static bool Jp2_ReadFTYP(Jp2Stream *pJp2, CReader *pStream)
	{
		PCommon pCodecInfo = pJp2->pCodecInfo;
		Jp2Box oBox;
		Jp2_ReadBoxHeader(pCodecInfo, pStream, &oBox);

		if (JP2_FTYP != oBox.nType)
		{
			Event_Message(EVT_ERROR, "Expected FTYP Marker\n");
			return false;
		}

		pJp2->nBrand      = pStream->Read(4); // BR
		pJp2->nMinVersion = pStream->Read(4); // MinV
		pJp2->nCompListLength = (oBox.nLength - 16) / 4;
		pJp2->pCompList = (unsigned int *)Malloc(pJp2->nCompListLength * sizeof(unsigned int));
		if (!pJp2->pCompList)
		{
			pJp2->pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
			return false;
		}

		pStream->Read((BYTE*)pJp2->pCompList, pJp2->nCompListLength * sizeof(unsigned int));
		//for ( unsigned int nIndex = 0; nIndex < (int)pJp2->nCompListLength; nIndex++ ) 
		//{
		//	pJp2->pCompList[nIndex] = pStream->Read( 4 ); // CLi
		//}

		if (pStream->Tell() - oBox.nInitPos != oBox.nLength)
		{
			Event_Message(EVT_ERROR, "Error with FTYP Box\n");
			return false;
		}

		return true;
	}

	static int  Jp2_WriteJP2C(Jp2Stream *pJp2, CReader *pStream, Image *pImage, char *sIndex)
	{
		J2kCodestream *pJ2k = pJp2->pJ2k;
		Jp2Box oBox;
		oBox.nInitPos =  pStream->Tell();
		pStream->Skip(4);
		pStream->Write(JP2_JP2C, 4); // JP2C

		// J2K
		unsigned int nJ2kCodestreamOffset =  pStream->Tell();
		if (!J2k_Encode(pJ2k, pStream, pImage, sIndex))
		{
			Event_Message(EVT_ERROR, "Failed to encode image\n");
			return 0;
		}
		unsigned int nJ2kCodestreamLength =  pStream->Tell() - nJ2kCodestreamOffset;

		pJp2->nJ2kCodestreamOffset = nJ2kCodestreamOffset;
		pJp2->nJ2kCodestreamLength = nJ2kCodestreamLength;

		oBox.nLength = 8 + pJp2->nJ2kCodestreamLength;
		pStream->Seek(oBox.nInitPos);
		pStream->Write(oBox.nLength, 4); // L
		pStream->Seek(oBox.nInitPos + oBox.nLength);

		return oBox.nLength;
	}

	static bool Jp2_ReadJP2C(Jp2Stream *pJp2, CReader *pStream, unsigned int *pnJ2kCodestreamLength, unsigned int *pnJ2kCodestreamOffset)
	{
		Jp2Box oBox;
		PCommon pCodecInfo = pJp2->pCodecInfo;
		Jp2_ReadBoxHeader(pCodecInfo, pStream, &oBox);
		do
		{
			if (JP2_JP2C != oBox.nType)
			{
				pStream->Skip(oBox.nLength - 8);
				Jp2_ReadBoxHeader(pCodecInfo, pStream, &oBox);
			}
		} while (JP2_JP2C != oBox.nType);

		*pnJ2kCodestreamOffset =  pStream->Tell();
		*pnJ2kCodestreamLength = oBox.nLength - 8;

		return true;
	}

	static void Jp2_WriteJP(CReader *pStream)
	{
		Jp2Box oBox;
		oBox.nInitPos =  pStream->Tell();
		pStream->Skip(4);
		pStream->Write(JP2_JP, 4); // JP2 signature
		pStream->Write(0x0d0a870a, 4);

		oBox.nLength =  pStream->Tell() - oBox.nInitPos;
		pStream->Seek(oBox.nInitPos);
		pStream->Write(oBox.nLength, 4); // L
		pStream->Seek(oBox.nInitPos + oBox.nLength);
	}

	static bool Jp2_ReadJP(Jp2Stream *pJp2, CReader *pStream)
	{
		Jp2Box oBox;
		PCommon pCodecInfo = pJp2->pCodecInfo;
		Jp2_ReadBoxHeader(pCodecInfo, pStream, &oBox);
		if (JP2_JP != oBox.nType)
		{
			Event_Message(EVT_ERROR, "Expected JP Marker\n");
			return false;
		}
		if (0x0d0a870a != pStream->Read(4))
		{
			Event_Message(EVT_ERROR, "Error with JP Marker\n");
			return false;
		}
		if (pStream->Tell() - oBox.nInitPos != oBox.nLength)
		{
			Event_Message(EVT_ERROR, "Error with JP Box size\n");
			return false;
		}

		return true;
	}


	static bool Jp2_ReadStruct(Jp2Stream *pJp2, CReader *pStream)
	{
		if (!Jp2_ReadJP(pJp2, pStream))
			return false;
		if (!Jp2_ReadFTYP(pJp2, pStream))
			return false;
		if (!Jp2_ReadJP2H(pJp2, pStream))
			return false;
		if (!Jp2_ReadJP2C(pJp2, pStream, &pJp2->nJ2kCodestreamLength, &pJp2->nJ2kCodestreamOffset))
			return false;

		return true;
	}



	//-------------------------------------------------------------------------------------------------------------------------------
	// Декодирование потока Jp2
	//-------------------------------------------------------------------------------------------------------------------------------
	void       Jp2_DestroyDecompress(Jp2Stream *pJp2)
	{
		if (pJp2)
		{
			J2k_DestroyDecompress(pJp2->pJ2k);

			Free(pJp2->oColor.icc_profile_buf);
			Free(pJp2->pComponents);
			Free(pJp2->pCompList);
			Free(pJp2);
		}
	}

	Jp2Stream* Jp2_CreateDecompress(PCommon pCodecInfo)
	{
		Jp2Stream *pJp2 = (Jp2Stream*)Malloc(sizeof(Jp2Stream));
		if (pJp2)
		{
			pJp2->pCodecInfo = pCodecInfo;
			pJp2->pJ2k = J2k_CreateDecompress(pCodecInfo);

			if (NULL == pJp2->pJ2k)
			{
				Jp2_DestroyDecompress(pJp2);
				return NULL;
			}
		}
		else
		{
			pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
		}
		return pJp2;
	}

	void       Jp2_SetupDecoder(Jp2Stream *pJp2, DecoderParams *pParameters)
	{
		J2k_SetupDecoder(pJp2->pJ2k, pParameters);
	}

	Image*     Jp2_Decode(Jp2Stream *pJp2, CReader *pStream)
	{
		if (!pJp2 || !pStream)
		{
			return NULL;
		}

		PCommon pCodecInfo = pJp2->pCodecInfo;

		// Декодируем JP2
		if (!Jp2_ReadStruct(pJp2, pStream))
		{
			Event_Message(EVT_ERROR, "Failed to decode jp2 structure\n");
			return NULL;
		}

		// Декодируем J2K
		Image *pImage = J2k_Decode(pJp2->pJ2k, pStream);
		if (!pImage)
		{
			Event_Message(EVT_ERROR, "Failed to decode J2K image\n");
		}

		////приведение цветовой схемы..????

		return pImage;
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// Кодирование в поток Jp2
	//-------------------------------------------------------------------------------------------------------------------------------
	void       Jp2_DestroyCompress(Jp2Stream *pJp2)
	{
		if (pJp2)
		{
			J2k_DestroyCompress(pJp2->pJ2k);

			Free(pJp2->pComponents);
			Free(pJp2->pCompList);
			Free(pJp2);
		}
	}

	Jp2Stream* Jp2_CreateCompress(PCommon pCodecInfo)
	{
		Jp2Stream *pJp2 = (Jp2Stream*)Malloc(sizeof(Jp2Stream));
		if (pJp2)
		{
			pJp2->pCodecInfo = pCodecInfo;
			pJp2->pJ2k = J2k_CreateCompress(pCodecInfo);
			if (NULL == pJp2->pJ2k)
			{
				Jp2_DestroyCompress(pJp2);
				return NULL;
			}
		}
		else
		{
			pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
		}
		return pJp2;
	}

	void       Jp2_SetupEncoder(Jp2Stream *pJp2, EncoderParams *pParameters, Image *pImage)
	{
		if (!pJp2 || !pParameters || !pImage)
			return;

		if (pImage->nCsiz < 1 || pImage->nCsiz > 16384)
		{
			Event_Message(EVT_ERROR, "Invalid number of components specified while setting up JP2 encoder\n");
			return;
		}

		J2k_SetupEncoder(pJp2->pJ2k, pParameters, pImage);
		if (JP2_ERROR_NO_ERROR != pJp2->pCodecInfo->nErrorCode)
			return;

		// Profile box
		pJp2->nBrand          = JP2_JP2; // BR
		pJp2->nMinVersion     = 0;       // MinV
		pJp2->nCompListLength = 1;
		pJp2->pCompList       = (unsigned int*)Malloc(pJp2->nCompListLength * sizeof(unsigned int));
		if (!pJp2->pCompList)
		{
			pJp2->pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
			return;
		}
		pJp2->pCompList[0]    = JP2_JP2; // CL0: JP2

		// Image Header box
		pJp2->nComponentsCount = pImage->nCsiz; // NC
		pJp2->pComponents      = (Jp2Component*)Malloc(pJp2->nComponentsCount * sizeof(Jp2Component));
		if (!pJp2->pComponents)
		{
			Free(pJp2->pCompList);
			pJp2->pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
			return;
		}
		pJp2->nHeight          = pImage->nYsiz - pImage->nYOsiz; // HEIGHT
		pJp2->nWidth           = pImage->nXsiz - pImage->nXOsiz; // WIDTH

		int nDepth0 = pImage->pComponents[0].nPrecision - 1;
		int nSign    = pImage->pComponents[0].nSigned;
		pJp2->nBPC             = nDepth0 + (nSign << 7); // BPC

		for (int nIndex = 1; nIndex < pImage->nCsiz; nIndex++)
		{
			int depth = pImage->pComponents[nIndex].nPrecision - 1;
			nSign = pImage->pComponents[nIndex].nSigned;
			if (nDepth0 != depth)
				pJp2->nBPC = 255;
		}
		pJp2->nCompressionType = 7; // C (всегда 7)
		pJp2->nColorSpaceUnk   = 0; // UnkC
		pJp2->nIPR             = 0; // IRP

		// BitsPerComponent box
		for (int nIndex = 0; nIndex < pImage->nCsiz; nIndex++)
		{
			pJp2->pComponents[nIndex].nBPCC = pImage->pComponents[nIndex].nPrecision - 1 + (pImage->pComponents[nIndex].nSigned << 7);
		}

		// Colour Specification box
		if ((pImage->nCsiz == 1 || pImage->nCsiz == 3) && (pJp2->nBPC != 255))
		{
			pJp2->nMethod = 1; // METH: Enumerated colourspace
		}
		else
		{
			pJp2->nMethod = 2; // METH: Restricted ICC profile
		}
		if (pJp2->nMethod == 1)
		{
			if (pImage->eColorSpace == 1)
				pJp2->nEnumCS = 16; // sRGB ( 61966–2–1 )
			else if (pImage->eColorSpace == 2)
				pJp2->nEnumCS = 17; // Greyscale
			else if (pImage->eColorSpace == 3)
				pJp2->nEnumCS = 18; // YUV
		}
		else
		{
			pJp2->nEnumCS = 0; // PROFILE
		}
		pJp2->nPrec   = 0; // PREC
		pJp2->nApprox = 0; // APPROX

	}

	bool       Jp2_Encode(Jp2Stream *pJp2, CReader *pStream, Image *pImage, char *sIndex)
	{
		// JP2 encoding

		// JPEG 2000 Signature box
		Jp2_WriteJP(pStream);
		// File Type box
		Jp2_WriteFTYP(pJp2, pStream);
		// JP2 Header box
		Jp2_WriteJP2H(pJp2, pStream);

		// J2K encoding
		if (!Jp2_WriteJP2C(pJp2, pStream, pImage, sIndex))
		{
			Event_Message(EVT_ERROR, "Failed to encode image\n");
			return false;
		}

		return true;
	}

}
