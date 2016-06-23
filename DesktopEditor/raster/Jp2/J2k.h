#pragma once
// Функции в данном файле предназначены для чтения/записи различных частей JPEG2000-codestream: маркеры и данные
//-------------------------------------------------------------------------------------------------------------------------------

#include "math.h"

#include "Types.h"
#include "Reader.h"
#include "Tile.h"
#include "Image.h"
#include "Jpt.h"

namespace Jpeg2000
{
	//-------------------------------------------------------------------------------------------------------------------------------
	static void J2k_DumpImage(FILE *pFile, Image *pImage)
	{
		fprintf(pFile, "Image\n");
		fprintf(pFile, "{\n");
		fprintf(pFile, "    XOffset = %d, YOffset = %d, Xsiz = %d, Ysiz = %d\n", pImage->nXOsiz, pImage->nYOsiz, pImage->nXsiz, pImage->nYsiz);
		fprintf(pFile, "    ComponentsCount = %d\n", pImage->nCsiz);
		for (int nCurComponent = 0; nCurComponent < pImage->nCsiz; nCurComponent++)
		{
			ImageComponent *pComponent = &pImage->pComponents[nCurComponent];
			fprintf(pFile, "    ComponentIndex %d\n", nCurComponent);
			fprintf(pFile, "    {\n");
			fprintf(pFile, "        XRsiz = %d, YRsiz = %d\n", pComponent->nXRsiz, pComponent->nYRsiz);
			fprintf(pFile, "        Precision = %d\n", pComponent->nPrecision);
			fprintf(pFile, "        Signed = %d\n", pComponent->nSigned);
			fprintf(pFile, "    }\n");
		}
		fprintf(pFile, "}\n");
	}

	static void J2k_DumpCodingParams(FILE *pFile, Image *pImage, CodingParams *pCodingParams)
	{
		fprintf(pFile, "<CodingParameters ");
		fprintf(pFile, "tx0='%d' ty0='%d' ", pCodingParams->nXTOsiz, pCodingParams->nYTOsiz);
		fprintf(pFile, "tdx='%d' tdy='%d' ", pCodingParams->nXTsiz, pCodingParams->nYTsiz);
		fprintf(pFile, "tw='%d' th='%d'>\n", pCodingParams->nXTilesCount, pCodingParams->nYTilesCount);

		for (int nTileIndex = 0; nTileIndex < pCodingParams->nXTilesCount * pCodingParams->nYTilesCount; nTileIndex++)
		{
			TileCodingParams *pTCP = &pCodingParams->pTCP[nTileIndex];
			fprintf(pFile, "<Tile Index='%d' ", nTileIndex);
			fprintf(pFile, "CodingStyle='%x' ", pTCP->nCodingStyle);
			fprintf(pFile, "ProgressionOrder='%d' ", pTCP->eProgOrder);
			fprintf(pFile, "LayersCount='%d' ", pTCP->nLayersCount);
			fprintf(pFile, "MultiComponentTransform='%d' ", pTCP->nMCT);
			fprintf(pFile, "Rates='");
			for (int nLayerIndex = 0; nLayerIndex < pTCP->nLayersCount; nLayerIndex++)
			{
				fprintf(pFile, "%.1f ", pTCP->afRates[nLayerIndex]);
			}
			fprintf(pFile, "'>\n");
			for (int nCurComponent = 0; nCurComponent < pImage->nCsiz; nCurComponent++)
			{
				TileCompCodingParams *pTCCP = &pTCP->pTCCP[nCurComponent];
				fprintf(pFile, "<Component Index='%d'>\n", nCurComponent);
				fprintf(pFile, "<CodingStyle Value='%x'/>\n", pTCCP->nCodingStyle);
				fprintf(pFile, "<ResolutionsCount Value='%d'/>\n", pTCCP->nResolutionsCount);
				fprintf(pFile, "<CodeBlockWidth Value='%d'/>\n", pTCCP->nCodeBlockWidth);
				fprintf(pFile, "<CodeBlockHeight Value='%d'/>\n", pTCCP->nCodeBlockHeight);
				fprintf(pFile, "<CodeBlockStyle Value='%x'/>\n", pTCCP->nCodeBlockStyle);
				fprintf(pFile, "<TransformID Value='%d'/>\n", pTCCP->nTransformID);
				fprintf(pFile, "<QuantStyle Value='%d'/>\n", pTCCP->nQuantStyle);
				fprintf(pFile, "<GuardBitsCount Value='%d'/>\n", pTCCP->nGuardBitsCount);
				fprintf(pFile, "<ROIShift Value='%d'/>\n", pTCCP->nROIShift);
				fprintf(pFile, "<StepSizes Value='");

				int nBandsCount = pTCCP->nQuantStyle == J2K_CCP_QNTSTY_SIQNT ? 1 : pTCCP->nResolutionsCount * 3 - 2;
				for (int nBandIndex = 0; nBandIndex < nBandsCount; nBandIndex++)
				{
					fprintf(pFile, "(%d,%d) ", pTCCP->aoStepSizes[nBandIndex].nMantissa, pTCCP->aoStepSizes[nBandIndex].nExponent);
				}
				fprintf(pFile, "'/>");

				if (pTCCP->nCodingStyle & J2K_CCP_CSTY_PRT)
				{
					fprintf(pFile, "<PartitionWidth Value='");
					for (int nResIndex = 0; nResIndex < pTCCP->nResolutionsCount; nResIndex++)
					{
						fprintf(pFile, "(%d)", pTCCP->anPrecinctWidth[nResIndex]);
					}
					fprintf(pFile, "'/>");
					fprintf(pFile, "<PartitionHeight Value='");
					for (int nResIndex = 0; nResIndex < pTCCP->nResolutionsCount; nResIndex++)
					{
						fprintf(pFile, "(%d)", pTCCP->anPrecinctHeight[nResIndex]);
					}
					fprintf(pFile, "'/>");
				}
				fprintf(pFile, "</Component>\n");
			}
			fprintf(pFile, "</Tile>\n");
		}
		fprintf(pFile, "</CodingParameters>\n");
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// Вспомогательные функции
	//-------------------------------------------------------------------------------------------------------------------------------
	static void J2k_WriteSOC(J2kCodestream *pJ2k)
	{
		CReader *pStream = pJ2k->pStreamIO;
		pStream->Write(J2K_MS_SOC, 2);
	}

	static void J2k_ReadSOC(J2kCodestream *pJ2k)
	{
		pJ2k->nState = j2kstateMHSIZ;
	}

	static void J2k_WriteSIZ(J2kCodestream *pJ2k)
	{
		CReader             *pStream = pJ2k->pStreamIO;
		Image               *pImage = pJ2k->pImage;
		CodingParams *pCodingParams = pJ2k->pCodingParams;

		pStream->Write(J2K_MS_SIZ, 2); // SIZ

		int nLsizPos =  pStream->Tell();
		pStream->Skip(2);

		pStream->Write(0, 2); // Rsiz (capabilities)
		pStream->Write(pImage->nXsiz, 4); // Xsiz
		pStream->Write(pImage->nYsiz, 4); // Ysiz
		pStream->Write(pImage->nXOsiz, 4); // X0siz
		pStream->Write(pImage->nYOsiz, 4); // Y0siz
		pStream->Write(pCodingParams->nXTsiz, 4); // XTsiz
		pStream->Write(pCodingParams->nYTsiz, 4); // YTsiz
		pStream->Write(pCodingParams->nXTOsiz, 4); // XT0siz
		pStream->Write(pCodingParams->nYTOsiz, 4); // YT0siz
		pStream->Write(pImage->nCsiz, 2); // Csiz

		for (int nIndex = 0; nIndex < pImage->nCsiz; nIndex++)
		{
			pStream->Write(pImage->pComponents[nIndex].nPrecision - 1 + (pImage->pComponents[nIndex].nSigned << 7), 1); // Ssiz_i
			pStream->Write(pImage->pComponents[nIndex].nXRsiz, 1); // XRsiz_i
			pStream->Write(pImage->pComponents[nIndex].nYRsiz, 1); // YRsiz_i
		}
		int nLen =  pStream->Tell() - nLsizPos;
		pStream->Seek(nLsizPos);
		pStream->Write(nLen, 2); // Lsiz
		pStream->Seek(nLsizPos + nLen);
	}

	static void J2k_ReadSIZ(J2kCodestream *pJ2k)
	{
		CReader            *pStream = pJ2k->pStreamIO;
		Image               *pImage = pJ2k->pImage;
		CodingParams *pCodingParams = pJ2k->pCodingParams;

		int nLen = pStream->Read(2); // Lsiz

		pStream->Read(2); // Rsiz (capabilities)

		pImage->nXsiz  = pStream->Read(4); // Xsiz
		pImage->nYsiz  = pStream->Read(4); // Ysiz
		pImage->nXOsiz = pStream->Read(4); // X0siz
		pImage->nYOsiz = pStream->Read(4); // Y0siz

		pCodingParams->nXTsiz  = pStream->Read(4); // XTsiz
		pCodingParams->nYTsiz  = pStream->Read(4); // YTsiz
		pCodingParams->nXTOsiz = pStream->Read(4); // XT0siz
		pCodingParams->nYTOsiz = pStream->Read(4); // YT0siz

		pImage->nCsiz = pStream->Read(2); // Csiz
		pImage->pComponents = (ImageComponent *)Malloc(pImage->nCsiz * sizeof(ImageComponent));
		if (!pImage->pComponents)
		{
			pJ2k->pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
			return;
		}

		for (int nIndex = 0; nIndex < pImage->nCsiz; nIndex++)
		{
			int nTemp = pStream->Read(1); // Ssiz_i
			pImage->pComponents[nIndex].nPrecision = (nTemp & 0x7f) + 1;
			pImage->pComponents[nIndex].nSigned    = nTemp >> 7;
			pImage->pComponents[nIndex].nXRsiz     = pStream->Read(1); // XRsiz_i
			pImage->pComponents[nIndex].nYRsiz     = pStream->Read(1); // YRsiz_i

			int nWidth  = CeilDiv(pImage->nXsiz - pImage->nXOsiz, pImage->pComponents[nIndex].nXRsiz);
			int nHeight = CeilDiv(pImage->nYsiz - pImage->nYOsiz, pImage->pComponents[nIndex].nYRsiz);

			pImage->pComponents[nIndex].nDecodedResCount = 0;
			pImage->pComponents[nIndex].nFactorDiv2      = 0;
		}

		// Считаем количество тайлов по Х и Y
		pCodingParams->nXTilesCount = CeilDiv(pImage->nXsiz - pCodingParams->nXTOsiz, pCodingParams->nXTsiz);
		pCodingParams->nYTilesCount = CeilDiv(pImage->nYsiz - pCodingParams->nYTOsiz, pCodingParams->nYTsiz);

		pCodingParams->pTCP = (TileCodingParams *)Malloc(pCodingParams->nXTilesCount * pCodingParams->nYTilesCount * sizeof(TileCodingParams));
		if (!pCodingParams->pTCP)
		{
			Free(pImage->pComponents);
			pJ2k->pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
			return;
		}

		pCodingParams->pTileIndex = (int *)Malloc(pCodingParams->nXTilesCount * pCodingParams->nYTilesCount * sizeof(int));
		if (!pCodingParams->pTileIndex)
		{
			Free(pImage->pComponents);
			Free(pCodingParams->pTCP);
			pJ2k->pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
			return;
		}

		pCodingParams->nTileIndexSize = 0;

		for (int nIndex = 0; nIndex < pCodingParams->nXTilesCount * pCodingParams->nYTilesCount; nIndex++)
		{
			pCodingParams->pTCP[nIndex].nUsePOC    = 0;
			pCodingParams->pTCP[nIndex].nPOCsCount = 0;
			pCodingParams->pTCP[nIndex].nFirst     = 1;
		}

		// Обнуляем данные для PPM маркера
		pCodingParams->nPPM          = 0;
		pCodingParams->pPPMData      = NULL;
		pCodingParams->pPPMDataFirst = NULL;
		pCodingParams->nPPMPrevious  = 0;
		pCodingParams->nPPMStore     = 0;

		pJ2k->pDefaultTCP->pTCCP = (TileCompCodingParams *)Malloc(sizeof(TileCompCodingParams) * pImage->nCsiz);
		if (!pJ2k->pDefaultTCP->pTCCP)
		{
			Free(pImage->pComponents);
			Free(pCodingParams->pTCP);
			Free(pCodingParams->pTileIndex);
			pJ2k->pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
			return;
		}

		for (int nIndex = 0; nIndex < pCodingParams->nXTilesCount * pCodingParams->nYTilesCount; nIndex++)
		{
			pCodingParams->pTCP[nIndex].pTCCP = (TileCompCodingParams *)Malloc(sizeof(TileCompCodingParams) * pImage->nCsiz);
			if (!pCodingParams->pTCP[nIndex].pTCCP)
			{
				for (int nI = 0; nI < nIndex; nI++)
				{
					Free(pCodingParams->pTCP[nI].pTCCP);
					Free(pImage->pComponents);
					Free(pCodingParams->pTCP);
					Free(pCodingParams->pTileIndex);
					Free(pJ2k->pDefaultTCP->pTCCP);
					pJ2k->pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
					return;
				}
			}
		}
		pJ2k->ppTileData = (unsigned char **)Malloc(pCodingParams->nXTilesCount * pCodingParams->nYTilesCount * sizeof(unsigned char *));
		if (!pJ2k->ppTileData)
		{
			for (int nIndex = 0; nIndex < pCodingParams->nXTilesCount * pCodingParams->nYTilesCount; nIndex++)
			{
				Free(pCodingParams->pTCP[nIndex].pTCCP);
			}
			Free(pImage->pComponents);
			Free(pCodingParams->pTCP);
			Free(pCodingParams->pTileIndex);
			Free(pJ2k->pDefaultTCP->pTCCP);
			Free(pJ2k->pDefaultTCP);
			pJ2k->pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
			return;
		}

		pJ2k->pTileLen   = (int *)Malloc(pCodingParams->nXTilesCount * pCodingParams->nYTilesCount * sizeof(int));
		if (!pJ2k->pTileLen)
		{
			for (int nIndex = 0; nIndex < pCodingParams->nXTilesCount * pCodingParams->nYTilesCount; nIndex++)
			{
				Free(pCodingParams->pTCP[nIndex].pTCCP);
			}
			Free(pImage->pComponents);
			Free(pCodingParams->pTCP);
			Free(pCodingParams->pTileIndex);
			Free(pJ2k->pDefaultTCP->pTCCP);
			Free(pJ2k->pDefaultTCP);
			Free(pJ2k->ppTileData);
			pJ2k->pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
			return;
		}
		pJ2k->nState = j2kstateMH;
	}

	static void J2k_WriteCOM(J2kCodestream *pJ2k)
	{
		if (pJ2k->pCodingParams->sComment)
		{
			CReader *pStream = pJ2k->pStreamIO;
			char  *sComment = pJ2k->pCodingParams->sComment;

			pStream->Write(J2K_MS_COM, 2);
			int nLcomPos =  pStream->Tell();
			pStream->Skip(2);
			pStream->Write(0, 2);  // Rcom
			for (unsigned int nIndex = 0; nIndex < strlen(sComment); nIndex++)
			{
				pStream->Write(sComment[nIndex], 1); // Ccom_i
			}
			int nLen =  pStream->Tell() - nLcomPos;
			pStream->Seek(nLcomPos);
			pStream->Write(nLen, 2); // Lcom
			pStream->Seek(nLcomPos + nLen);
		}
	}

	static void J2k_ReadCOM(J2kCodestream *pJ2k)
	{
		CReader *pStream = pJ2k->pStreamIO;

		int nLen = pStream->Read(2);

		// TO DO: Сделать нормальное чтение
		pStream->Skip(nLen - 2);
	}

	static void J2k_WriteCOX(J2kCodestream *pJ2k, int nCurComponent)
	{
		CodingParams *pCodingParams = pJ2k->pCodingParams;
		TileCodingParams      *pTCP = &pCodingParams->pTCP[pJ2k->nCurTileIndex];
		TileCompCodingParams *pTCCP = &pTCP->pTCCP[nCurComponent];
		CReader *pStream = pJ2k->pStreamIO;

		pStream->Write(pTCCP->nResolutionsCount - 1, 1); // SPcox (D)
		pStream->Write(pTCCP->nCodeBlockWidth - 2, 1); // SPcox (E)
		pStream->Write(pTCCP->nCodeBlockHeight - 2, 1); // SPcox (F)
		pStream->Write(pTCCP->nCodeBlockStyle, 1); // SPcox (G)
		pStream->Write(pTCCP->nTransformID, 1); // SPcox (H)

		if (pTCCP->nCodingStyle & J2K_CCP_CSTY_PRT)
		{
			for (int nIndex = 0; nIndex < pTCCP->nResolutionsCount; nIndex++)
			{
				pStream->Write(pTCCP->anPrecinctWidth[nIndex] + (pTCCP->anPrecinctHeight[nIndex] << 4), 1); // SPcox (I_i)
			}
		}
	}

	static void J2k_ReadCOX(J2kCodestream *pJ2k, int nCurComponent)
	{
		CodingParams *pCodingParams = pJ2k->pCodingParams;
		TileCodingParams      *pTCP = pJ2k->nState == j2kstateTPH ? &pCodingParams->pTCP[pJ2k->nCurTileIndex] : pJ2k->pDefaultTCP;
		TileCompCodingParams *pTCCP = &pTCP->pTCCP[nCurComponent];
		CReader *pStream = pJ2k->pStreamIO;

		pTCCP->nResolutionsCount = pStream->Read(1) + 1; // SPcox (D)

		/* check the reduce value */
        pCodingParams->nReduceFactor = (std::min)((pTCCP->nResolutionsCount) - 1, pCodingParams->nReduceFactor);

		pTCCP->nCodeBlockWidth  = pStream->Read(1) + 2; // SPcox (E)
		pTCCP->nCodeBlockHeight = pStream->Read(1) + 2; // SPcox (F)
		pTCCP->nCodeBlockStyle  = pStream->Read(1);     // SPcox (G)
		pTCCP->nTransformID     = pStream->Read(1);     // SPcox (H)

		if (pTCCP->nCodingStyle & J2K_CP_CSTY_PRT)
		{
			for (int nIndex = 0; nIndex < pTCCP->nResolutionsCount; nIndex++)
			{
				int nTemp = pStream->Read(1); // SPcox (I_i)
				pTCCP->anPrecinctWidth[nIndex]  = nTemp & 0xf;
				pTCCP->anPrecinctHeight[nIndex] = nTemp >> 4;
			}
		}
	}

	static void J2k_WriteCOD(J2kCodestream *pJ2k)
	{
		CReader *pStream = pJ2k->pStreamIO;

		pStream->Write(J2K_MS_COD, 2); // COD

		int nLcodPos =  pStream->Tell();
		pStream->Skip(2);

		CodingParams *pCodingParams = pJ2k->pCodingParams;
		TileCodingParams *pTCP = &pCodingParams->pTCP[pJ2k->nCurTileIndex];

		pStream->Write(pTCP->nCodingStyle, 1);	// Scod
		pStream->Write(pTCP->eProgOrder, 1); // SGcod (A)
		pStream->Write(pTCP->nLayersCount, 2); // SGcod (B)
		pStream->Write(pTCP->nMCT, 1); // SGcod (C)

		J2k_WriteCOX(pJ2k, 0);
		int nLen =  pStream->Tell() - nLcodPos;
		pStream->Seek(nLcodPos);
		pStream->Write(nLen, 2); // Lcod
		pStream->Seek(nLcodPos + nLen);
	}

	//static void J2k_ReadEPC(J2kCodestream *pJ2k) 
	//{
	// 	CReader *pStream = pJ2k->pStreamIO;
	//
	//	unsigned long int DL, Lepcp, Pcrcp, l;
	//    unsigned short int Lepc, Pcrc = 0x0000;
	//    unsigned char Pepc;     
	//
	//	const char *ans1;
	//
	//    /* Simply read the EPC parameters */
	//    Lepcp = pStream->Tell();// cio_tell(cio);
	//	Lepc = pStream->Read(2);//cio_read(cio, 2);
	//    Pcrcp = pStream->Tell();
	//	pStream->Skip(2); /* Pcrc */
	//    DL = cpStream->Read( 4);
	//    Pepc = pStream->Read( 1);
	//
	//        /* compute Pcrc */
	//		pStream->Seek(Lepcp - 2);
	//
	//        /* Marker */
	//        jpwl_updateCRC16(&Pcrc, (unsigned char) pStream->Read( 1)); 
	//        jpwl_updateCRC16(&Pcrc, (unsigned char) pStream->Read( 1)); 
	//
	//        /* Length */
	//        jpwl_updateCRC16(&Pcrc, (unsigned char) pStream->Read( 1)); 
	//        jpwl_updateCRC16(&Pcrc, (unsigned char) pStream->Read( 1)); 
	//
	//        /* skip Pcrc */
	//        pStream->Skip( 2);
	//
	//        /* read all remaining */
	//        for (l = 4; l < Lepc; l++)
	//                jpwl_updateCRC16(&Pcrc, (unsigned char) pStream->Read(  1)); 
	//
	//    /* check Pcrc with the result */
	//    pStream->Seek Pcrcp);
	//    ans1 = (Pcrc == (unsigned short int)  pStream->Read( 2)) ? "crc-ok" : "crc-ko";
	//
	//	pStream->Seek(Lepcp + Lepc);  
	//}
	//static void J2k_ReadEPB(J2kCodestream *pJ2k) 
	//{
	// 	CReader *pStream = pJ2k->pStreamIO;
	//
	//	unsigned long int LDPepb, Pepb;
	//	unsigned short int Lepb;
	//	unsigned char Depb;
	//	char str1[25] = "";
	//	opj_bool status;
	//	static opj_bool first_in_tph = OPJ_TRUE;
	//	int type, pre_len, post_len;
	//	static unsigned char *redund = NULL;
	//
	//	/* B/W = 45, RGB = 51 */
	//        /*           SIZ   SIZ_FIELDS     SIZ_COMPS               FOLLOWING_MARKER */
	//        int skipnum = 2  +     38     + 3 * pJ2k->cp->exp_comps  +         2;
	//
	//        if (j2k->cp->correct) {
	//
	//        /* go back to EPB marker value */
	//        pStream->Seek(pStream->Tell() - 2);
	//
	//        /* we need to understand where we are */
	//        if (pJ2k->state == J2K_STATE_MH) {
	//                /* we are in MH */
	//                type = 0; /* MH */
	//                pre_len = skipnum; /* SOC+SIZ */
	//                post_len = -1; /* auto */
	//
	//        } else if ((pJ2k->state == J2K_STATE_TPH) && first_in_tph) {
	//                /* we are in TPH */
	//                type = 1; /* TPH */
	//                pre_len = 12; /* SOC+SIZ */
	//                first_in_tph = OPJ_FALSE;
	//                post_len = -1; /* auto */
	//
	//                } else {
	//                        /* we are elsewhere */
	//                        type = 2; /* other */
	//                        pre_len = 0; /* nada */
	//                        post_len = -1; /* auto */
	//
	//                }
	//
	//                /* call EPB corrector */
	//                /*printf("before %x, ", redund);*/
	//                status = jpwl_epb_correct(j2k,      /* J2K decompressor handle */
	//                                                                  cio->bp,  /* pointer to EPB in codestream buffer */
	//                                                                  type,     /* EPB type: MH */
	//                                                                  pre_len,  /* length of pre-data */
	//                                                                  post_len, /* length of post-data: -1 means auto */
	//                                                                  NULL,     /* do everything auto */
	//                                                                  &redund
	//                                                                 );
	//                /*printf("after %x\n", redund);*/
	//
	//                /* Read the (possibly corrected) EPB parameters */
	//                cio_skip(cio, 2);
	//                Lepb = cio_read(cio, 2);
	//                Depb = cio_read(cio, 1);
	//                LDPepb = cio_read(cio, 4);
	//                Pepb = cio_read(cio, 4);
	//
	//                if (!status) {
	//
	//                        opj_event_msg(j2k->cinfo, EVT_ERROR, "JPWL correction could not be performed\n");
	//
	//                        /* advance to EPB endpoint */
	//                        cio_skip(cio, Lepb + 2);  
	//
	//                        return;
	//                }
	//
	//                /* last in current header? */
	//                if (Depb & 0x40) {
	//                        redund = NULL; /* reset the pointer to L4 buffer */
	//                        first_in_tph = OPJ_TRUE;
	//                }
	//
	//                /* advance to EPB endpoint */
	//                cio_skip(cio, Lepb - 11);  
	//
	//        } else {
	//
	//                /* Simply read the EPB parameters */
	//                Lepb = cio_read(cio, 2);
	//                Depb = cio_read(cio, 1);
	//                LDPepb = cio_read(cio, 4);
	//                Pepb = cio_read(cio, 4);
	//
	//                /* What does Pepb tells us about the protection method? */
	//                if (((Pepb & 0xF0000000) >> 28) == 0)
	//                        sprintf(str1, "pred"); /* predefined */
	//                else if (((Pepb & 0xF0000000) >> 28) == 1)
	//                        sprintf(str1, "crc-%lu", 16 * ((Pepb & 0x00000001) + 1)); /* CRC mode */
	//                else if (((Pepb & 0xF0000000) >> 28) == 2)
	//                        sprintf(str1, "rs(%lu,32)", (Pepb & 0x0000FF00) >> 8); /* RS mode */
	//                else if (Pepb == 0xFFFFFFFF)
	//                        sprintf(str1, "nometh"); /* RS mode */
	//                else
	//                        sprintf(str1, "unknown"); /* unknown */
	//
	//                /* Now we write them to screen */
	//                opj_event_msg(j2k->cinfo, EVT_INFO,
	//                        "EPB(%d): (%sl, %sp, %u), %lu, %s\n",
	//                        cio_tell(cio) - 13,
	//                        (Depb & 0x40) ? "" : "n", /* latest EPB or not? */
	//                        (Depb & 0x80) ? "" : "n", /* packed or unpacked EPB? */
	//                        (Depb & 0x3F), /* EPB index value */
	//                        LDPepb, /*length of the data protected by the EPB */
	//                        str1); /* protection method */
	//
	//                cio_skip(cio, Lepb - 11);  
	//        }
	//}

	static void J2k_ReadCOD(J2kCodestream *pJ2k)
	{
		CReader *pStream = pJ2k->pStreamIO;
		CodingParams *pCodingParams = pJ2k->pCodingParams;
		TileCodingParams      *pTCP = pJ2k->nState == j2kstateTPH ? &pCodingParams->pTCP[pJ2k->nCurTileIndex] : pJ2k->pDefaultTCP;
		Image *pImage = pJ2k->pImage;

		int nLen           = pStream->Read(2); // Lcod
		pTCP->nCodingStyle = pStream->Read(1); // Scod
		pTCP->eProgOrder   = (ProgressionOrder)pStream->Read(1); // SGcod (A)
		pTCP->nLayersCount = pStream->Read(2); // SGcod (B)
		pTCP->nMCT         = pStream->Read(1); // SGcod (C)

		int nPos =  pStream->Tell();
		for (int nIndex = 0; nIndex < pImage->nCsiz; nIndex++)
		{
			pTCP->pTCCP[nIndex].nCodingStyle = pTCP->nCodingStyle & J2K_CP_CSTY_PRT;
			pStream->Seek(nPos);
			J2k_ReadCOX(pJ2k, nIndex);
		}
	}

	static void J2k_WriteCOC(J2kCodestream *pJ2k, int nCurComponent)
	{
		CodingParams *pCodingParams = pJ2k->pCodingParams;
		TileCodingParams *pTCP = &pCodingParams->pTCP[pJ2k->nCurTileIndex];
		Image *pImage = pJ2k->pImage;
		CReader *pStream = pJ2k->pStreamIO;

		pStream->Write(J2K_MS_COC, 2); // COC

		int nLcocPos =  pStream->Tell();
		pStream->Skip(2);

		pStream->Write(nCurComponent, pImage->nCsiz <= 256 ? 1 : 2); // Ccoc
		pStream->Write(pTCP->pTCCP[nCurComponent].nCodingStyle, 1);  // Scoc
		J2k_WriteCOX(pJ2k, nCurComponent);

		int nLen =  pStream->Tell() - nLcocPos;
		pStream->Seek(nLcocPos);
		pStream->Write(nLen, 2); // Lcoc
		pStream->Seek(nLcocPos + nLen);
	}

	static void J2k_ReadCOC(J2kCodestream *pJ2k)
	{
		CodingParams *pCodingParams = pJ2k->pCodingParams;
		TileCodingParams *pTCP = pJ2k->nState == j2kstateTPH ? &pCodingParams->pTCP[pJ2k->nCurTileIndex] : pJ2k->pDefaultTCP;
		Image *pImage = pJ2k->pImage;
		CReader *pStream = pJ2k->pStreamIO;

		int nLen = pStream->Read(2); // Lcoc
		int nCurComponent = pStream->Read(pImage->nCsiz <= 256 ? 1 : 2); // Ccoc
		pTCP->pTCCP[nCurComponent].nCodingStyle = pStream->Read(1);      // Scoc
		J2k_ReadCOX(pJ2k, nCurComponent);
	}

	static void J2k_WriteQCX(J2kCodestream *pJ2k, int nCurComponent)
	{
		CodingParams *pCodingParams = pJ2k->pCodingParams;
		TileCodingParams *pTCP = &pCodingParams->pTCP[pJ2k->nCurTileIndex];
		TileCompCodingParams *pTCCP = &pTCP->pTCCP[nCurComponent];
		CReader *pStream = pJ2k->pStreamIO;

		pStream->Write(pTCCP->nQuantStyle + (pTCCP->nGuardBitsCount << 5), 1); // Sqcx
		int nBandsCount = pTCCP->nQuantStyle == J2K_CCP_QNTSTY_SIQNT ? 1 : pTCCP->nResolutionsCount * 3 - 2;

		for (int nCurBand = 0; nCurBand < nBandsCount; nCurBand++)
		{
			int nExponent = pTCCP->aoStepSizes[nCurBand].nExponent;
			int nMantissa = pTCCP->aoStepSizes[nCurBand].nMantissa;

			if (pTCCP->nQuantStyle == J2K_CCP_QNTSTY_NOQNT)
			{
				pStream->Write(nExponent << 3, 1);// SPqcx_i
			}
			else
			{
				pStream->Write((nExponent << 11) + nMantissa, 2); // SPqcx_i
			}
		}
	}

	static void J2k_ReadQCX(J2kCodestream *pJ2k, int nCurComponent, int nLen)
	{
		CodingParams *pCodingParams = pJ2k->pCodingParams;
		TileCodingParams *pTCP = pJ2k->nState == j2kstateTPH ? &pCodingParams->pTCP[pJ2k->nCurTileIndex] : pJ2k->pDefaultTCP;
		TileCompCodingParams *pTCCP = &pTCP->pTCCP[nCurComponent];
		CReader *pStream = pJ2k->pStreamIO;

		int nTemp = pStream->Read(1); // Sqcx
		pTCCP->nQuantStyle = nTemp & 0x1f;
		pTCCP->nGuardBitsCount = nTemp >> 5;

		int nBandsCount = (pTCCP->nQuantStyle == J2K_CCP_QNTSTY_SIQNT) ? 1 : ((pTCCP->nQuantStyle == J2K_CCP_QNTSTY_NOQNT) ? nLen - 1 : (nLen - 1) / 2);

		for (int nCurBand = 0; nCurBand < nBandsCount; nCurBand++)
		{
			int nExponent = 0, nMantissa = 0;
			if (pTCCP->nQuantStyle == J2K_CCP_QNTSTY_NOQNT)
			{
				nExponent = pStream->Read(1) >> 3; // SPqcx_i
				nMantissa = 0;
			}
			else
			{
				nTemp = pStream->Read(2); // SPqcx_i
				nExponent = nTemp >> 11;
				nMantissa = nTemp & 0x7ff;
			}
			pTCCP->aoStepSizes[nCurBand].nExponent = nExponent;
			pTCCP->aoStepSizes[nCurBand].nMantissa = nMantissa;
		}

		// Add : Если тип квантования ScalarImplicit, тогда вычислим коэффициенты квантования для остальных Subbands
		if (pTCCP->nQuantStyle == J2K_CCP_QNTSTY_SIQNT)
		{
			for (int nCurBand = 1; nCurBand < J2K_MAXBANDS; nCurBand++)
			{
				pTCCP->aoStepSizes[nCurBand].nExponent = ((pTCCP->aoStepSizes[0].nExponent) - ((nCurBand - 1) / 3) > 0) ? (pTCCP->aoStepSizes[0].nExponent) - ((nCurBand - 1) / 3) : 0;
				pTCCP->aoStepSizes[nCurBand].nMantissa = pTCCP->aoStepSizes[0].nMantissa;
			}
		}
		// ddA
	}

	static void J2k_WriteQCD(J2kCodestream *pJ2k)
	{
		CReader *pStream = pJ2k->pStreamIO;

		pStream->Write(J2K_MS_QCD, 2); // QCD
		int nLqcdPos =  pStream->Tell();
		pStream->Skip(2);

		J2k_WriteQCX(pJ2k, 0);

		int nLen =  pStream->Tell() - nLqcdPos;
		pStream->Seek(nLqcdPos);
		pStream->Write(nLen, 2); // Lqcd
		pStream->Seek(nLqcdPos + nLen);
	}

	static void J2k_ReadQCD(J2kCodestream *pJ2k)
	{
		CReader *pStream = pJ2k->pStreamIO;
		Image *pImage = pJ2k->pImage;

		int nLen = pStream->Read(2); // Lqcd
		int nPos =  pStream->Tell();
		for (int nIndex = 0; nIndex < pImage->nCsiz; nIndex++)
		{
			pStream->Seek(nPos);
			J2k_ReadQCX(pJ2k, nIndex, nLen - 2);
		}
	}

	static void J2k_WriteQCC(J2kCodestream *pJ2k, int nCurComponent)
	{
		CReader *pStream = pJ2k->pStreamIO;

		pStream->Write(J2K_MS_QCC, 2); // QCC

		int nLqccPos =  pStream->Tell();
		pStream->Skip(2);

		pStream->Write(nCurComponent, pJ2k->pImage->nCsiz <= 256 ? 1 : 2); // Cqcc
		J2k_WriteQCX(pJ2k, nCurComponent);

		int nLen =  pStream->Tell() - nLqccPos;
		pStream->Seek(nLqccPos);
		pStream->Write(nLen, 2); // Lqcc
		pStream->Seek(nLqccPos + nLen);
	}

	static void J2k_ReadQCC(J2kCodestream *pJ2k)
	{
		int nComponentsCount = pJ2k->pImage->nCsiz;
		CReader *pStream = pJ2k->pStreamIO;

		int nLen = pStream->Read(2); // Lqcc
		int nCurComponent = pStream->Read(nComponentsCount <= 256 ? 1 : 2); // Cqcc
		J2k_ReadQCX(pJ2k, nCurComponent, nLen - 2 - (nComponentsCount <= 256 ? 1 : 2));
	}

	static void J2k_WritePOC(J2kCodestream *pJ2k)
	{
		int nComponentsCount = pJ2k->pImage->nCsiz;

		CodingParams *pCodingParams = pJ2k->pCodingParams;
		TileCodingParams *pTCP = &pCodingParams->pTCP[pJ2k->nCurTileIndex];
		TileCompCodingParams *pTCCP = &pTCP->pTCCP[0];
		CReader *pStream = pJ2k->pStreamIO;

		int nChangesCount = pTCP->nPOCsCount;
		pStream->Write(J2K_MS_POC, 2); // POC

		int nLen = 2 + (5 + 2 * (nComponentsCount <= 256 ? 1 : 2)) * nChangesCount;
		pStream->Write(nLen, 2); // Lpoc

		for (int nIndex = 0; nIndex < nChangesCount; nIndex++)
		{
			POC *pPOC = &pTCP->aoPOC[nIndex];
			pStream->Write(pPOC->nRSpoc, 1); // RSpoc_i
			pStream->Write(pPOC->nCSpoc, (nComponentsCount <= 256 ? 1 : 2)); // CSpoc_i

			pStream->Write(pPOC->nLYEpoc, 2); // LYEpoc_i
            pPOC->nLYEpoc = (std::min)(pPOC->nLYEpoc, pTCP->nLayersCount);

			pStream->Write(pPOC->nREpoc, 1); // REpoc_i
            pPOC->nREpoc = (std::min)(pPOC->nREpoc, pTCCP->nResolutionsCount);

			pStream->Write(pPOC->nCEpoc, (nComponentsCount <= 256 ? 1 : 2)); // CEpoc_i
            pPOC->nCEpoc = (std::min)(pPOC->nCEpoc, nComponentsCount);

			pStream->Write(pPOC->ePpoc, 1); // Ppoc_i
		}
	}

	static void J2k_ReadPOC(J2kCodestream *pJ2k)
	{
		int nComponentsCount = pJ2k->pImage->nCsiz;

		CodingParams *pCodingParams = pJ2k->pCodingParams;
		TileCodingParams *pTCP = pJ2k->nState == j2kstateTPH ? &pCodingParams->pTCP[pJ2k->nCurTileIndex] : pJ2k->pDefaultTCP;
		TileCompCodingParams *pTCCP = &pTCP->pTCCP[0];
		CReader *pStream = pJ2k->pStreamIO;

		int nOldPOC = pTCP->nUsePOC ? pTCP->nPOCsCount + 1 : 0;
		pTCP->nUsePOC = 1;

		int nLen = pStream->Read(2); // Lpoc
		int nChangesCount = (nLen - 2) / (5 + 2 * (nComponentsCount <= 256 ? 1 : 2));

		for (int nIndex = nOldPOC; nIndex < nChangesCount + nOldPOC; nIndex++)
		{
			POC *pPOC = &pTCP->aoPOC[nIndex];
			pPOC->nRSpoc  = pStream->Read(1); // RSpoc_i
			pPOC->nCSpoc  = pStream->Read(nComponentsCount <= 256 ? 1 : 2); // CSpoc_i
            pPOC->nLYEpoc = (std::min)(pStream->Read(2), (unsigned int)pTCP->nLayersCount); // LYEpoc_i
            pPOC->nREpoc  = (std::min)(pStream->Read(1), (unsigned int)pTCCP->nResolutionsCount); // REpoc_i
            pPOC->nCEpoc  = (std::min)(pStream->Read(nComponentsCount <= 256 ? 1 : 2), (unsigned int)nComponentsCount); // CEpoc_i
			pPOC->ePpoc   = (ProgressionOrder)pStream->Read(1); // Ppoc_i
		}

		pTCP->nPOCsCount = nChangesCount + nOldPOC - 1;
	}

	static void J2k_ReadCRG(J2kCodestream *pJ2k)
	{
		int nXcrg_i, nYcrg_i;

		CReader *pStream = pJ2k->pStreamIO;
		int nComponentsCount = pJ2k->pImage->nCsiz;

		int nLen = pStream->Read(2); // Lcrg
		for (int nIndex = 0; nIndex < nComponentsCount; nIndex++)
		{
			nXcrg_i = pStream->Read(2); // Xcrg_i
			nYcrg_i = pStream->Read(2); // Ycrg_i
		}
	}

	static void J2k_ReadTLM(J2kCodestream *pJ2k)
	{
		CReader *pStream = pJ2k->pStreamIO;

		int nLen  = pStream->Read(2); // Ltlm
		int nZtlm = pStream->Read(1); // Ztlm
		int nStlm = pStream->Read(1); // Stlm

		int nTlmSize = ((nStlm >> 4) & 0x01) + ((nStlm >> 4) & 0x02);
		int nPlmSize = (nStlm >> 6) & 0x01;

		int nTileTlm = (nLen - 4) / ((nPlmSize + 1) * 2 + nTlmSize);
		for (int nIndex = 0; nIndex < nTileTlm; nIndex++)
		{
			long int nTtlm_i = pStream->Read(nTlmSize);         // Ttlm_i
			long int nPtlm_i = pStream->Read(nPlmSize ? 4 : 2); // Ptlm_i
		}
	}

	static void J2k_ReadPLM(J2kCodestream *pJ2k)
	{
		int nPacketLen = 0;

		CReader *pStream = pJ2k->pStreamIO;

		int nLen  = pStream->Read(2); // Lplm
		int nZplm = pStream->Read(1); // Zplm
		nLen -= 3;
		while (nLen > 0)
		{
			int nNplm = pStream->Read(4); // Nplm
			nLen -= 4;
			for (int nIndex = nNplm; nIndex > 0; nIndex--)
			{
				int nAdd = pStream->Read(1);
				nLen--;

				nPacketLen = (nPacketLen << 7) + nAdd; // Iplm_ij
				if ((nAdd & 0x80) == 0)
				{
					// новый packet
					nPacketLen = 0;
				}
				if (nLen <= 0)
					break;
			}
		}
	}

	static void J2k_ReadPLT(J2kCodestream *pJ2k)
	{
		int nPacketLen = 0;

		CReader *pStream = pJ2k->pStreamIO;

		int nLen  = pStream->Read(2); // Lplt
		int nZplt = pStream->Read(1); // Zplt

		for (int nIndex = nLen - 3; nIndex > 0; nIndex--)
		{
			int nAdd = pStream->Read(1);
			nPacketLen = (nPacketLen << 7) + nAdd; // Iplt_i
			if ((nAdd & 0x80) == 0)
			{
				// новый packet
				nPacketLen = 0;
			}
		}
	}

	static void J2k_ReadPPM(J2kCodestream *pJ2k)
	{
		int nNppm;

		CodingParams *pCodingParams = pJ2k->pCodingParams;
		CReader *pStream = pJ2k->pStreamIO;

		int nLen  = pStream->Read(2); // Lppm
		pCodingParams->nPPM = 1;

		int nZppm = pStream->Read(1); // Zppm
		nLen -= 3;

		while (nLen > 0)
		{
			if (pCodingParams->nPPMPrevious == 0)
			{
				nNppm = pStream->Read(4); // Nppm
				nLen -= 4;
			}
			else
			{
				nNppm = pCodingParams->nPPMPrevious;
			}

			int nStore = pCodingParams->nPPMStore;
			if (nZppm == 0) // Первый PPM маркер
			{
				pCodingParams->pPPMData      = (unsigned char *)Malloc(nNppm * sizeof(unsigned char));
				if (!pCodingParams->pPPMData)
				{
					pJ2k->pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
					return;
				}
				pCodingParams->pPPMDataFirst = pCodingParams->pPPMData;
				pCodingParams->nPPMLength    = nNppm;
			}
			else // Не первый PPM маркер
			{
				BYTE *pPPMData_new = (BYTE *)Malloc((nNppm + pCodingParams->nPPMStore) * sizeof(unsigned char));
				if (!pPPMData_new)
				{
					pJ2k->pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
					return;
				}
				memcpy(pPPMData_new, pCodingParams->pPPMData, nNppm * sizeof(unsigned char));
				Free(pCodingParams->pPPMData);
				pCodingParams->pPPMData = pPPMData_new;

				pCodingParams->pPPMDataFirst = pCodingParams->pPPMData;
				pCodingParams->nPPMLength    = nNppm + pCodingParams->nPPMStore;
			}

			// Считываем остальные данные
			int nIndex;
			for (nIndex = nNppm; nIndex > 0; nIndex--)
			{
				pCodingParams->pPPMData[nStore] = pStream->Read(1);
				nStore++;
				nLen--;
				if (nLen == 0) // Случай, когда packet header не закончился в текущем маркере, но закончится в следующем
					break;
			}
			pCodingParams->nPPMPrevious = nIndex - 1;
			pCodingParams->nPPMStore    = nStore;
		}
	}

	static void J2k_ReadPPT(J2kCodestream *pJ2k)
	{
		CodingParams *pCodingParams = pJ2k->pCodingParams;
		TileCodingParams *pTCP = pCodingParams->pTCP + pJ2k->nCurTileIndex;
		CReader *pStream = pJ2k->pStreamIO;

		int nLen  = pStream->Read(2); // Lppt
		int nZppt = pStream->Read(1); // Zppt
		pTCP->nPPT = 1;

		if (nZppt == 0) // Первый PPT маркер 
		{
			pTCP->pPPTData      = (unsigned char *)Malloc((nLen - 3) * sizeof(unsigned char));
			if (!pTCP->pPPTData)
			{
				pJ2k->pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
				return;
			}
			pTCP->pPPTDataFirst = pTCP->pPPTData;
			pTCP->nPPTStore     = 0;
			pTCP->nPPTLength    = nLen - 3;
		}
		else // Не первый PPT маркер
		{
			BYTE *pPPTData_new =  (unsigned char *)Malloc((nLen - 3 + pTCP->nPPTStore) * sizeof(unsigned char));
			if (!pPPTData_new)
			{
				pJ2k->pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
				return;
			}
			memcpy(pPPTData_new, pTCP->pPPTData, (nLen - 3) * sizeof(unsigned char));
			Free(pTCP->pPPTData);
			pTCP->pPPTData      =pPPTData_new;

			pTCP->pPPTDataFirst = pTCP->pPPTData;
			pTCP->nPPTLength    = nLen - 3 + pTCP->nPPTStore;
		}

		int nStore = pTCP->nPPTStore;
		for (int nIndex = nLen - 3; nIndex > 0; nIndex--)
		{
			pTCP->pPPTData[nStore] = pStream->Read(1);
			nStore++;
		}
		pTCP->nPPTStore = nStore;
	}

	static void J2k_WriteSOT(J2kCodestream *pJ2k)
	{
		CReader *pStream = pJ2k->pStreamIO;

		pJ2k->nSOTStartPos =  pStream->Tell();

		pStream->Write(J2K_MS_SOT, 2); // SOT

		int nLsotPos =  pStream->Tell();
		pStream->Skip(2);

		pStream->Write(pJ2k->nCurTileIndex, 2); // Isot
		pStream->Skip(4);                       // Psot (пока оставляем место, сюда пишем в функции J2k_WriteSOD)
		pStream->Write(0, 1);                   // TPsot
		pStream->Write(1, 1);                   // TNsot

		int nLen =  pStream->Tell() - nLsotPos;
		pStream->Seek(nLsotPos);
		pStream->Write(nLen, 2); // Lsot
		pStream->Seek(nLsotPos + nLen);
	}

	static void J2k_ReadSOT(J2kCodestream *pJ2k)
	{
		char nStatus = 0;

		CodingParams *pCodingParams = pJ2k->pCodingParams;
		CReader *pStream = pJ2k->pStreamIO;

		int nLen       = pStream->Read(2); // Lsot
		int nTileIndex = pStream->Read(2); // Isot

		if (pCodingParams->nTileIndexSize == 0)
		{
			pCodingParams->pTileIndex[pCodingParams->nTileIndexSize] = nTileIndex;
			pCodingParams->nTileIndexSize++;
		}
		else
		{
			int nIndex = 0;
			while (nIndex < pCodingParams->nTileIndexSize && nStatus == 0)
			{
				nStatus = pCodingParams->pTileIndex[nIndex] == nTileIndex ? 1 : 0;
				nIndex++;
			}
			if (0 == nStatus)
			{
				pCodingParams->pTileIndex[pCodingParams->nTileIndexSize] = nTileIndex;
				pCodingParams->nTileIndexSize++;
			}
		}

		int nTotalLen = pStream->Read(4); // Psot

		if (!nTotalLen)
			nTotalLen = pStream->GetLeftSize() + 8;

		int nTilePartIndex  = pStream->Read(1); // TPsot
		int nTilePartsCount = pStream->Read(1); // TNsot

		pJ2k->nCurTileIndex = nTileIndex;
		//	pJ2k->pEndOfTile    = ByteIO_GetCurPos( pStream ) - 12 + nTotalLen;

		pJ2k->nSizeOfTile	= pStream->Tell() - 12 + nTotalLen;
		pJ2k->nState        = j2kstateTPH;

		TileCodingParams *pTCP = &pCodingParams->pTCP[pJ2k->nCurTileIndex];

		if (pTCP->nFirst == 1) // Инициализируем PPT
		{
			TileCompCodingParams *pTemp = pTCP->pTCCP;
			memcpy(pTCP, pJ2k->pDefaultTCP, sizeof(TileCodingParams));
			pTCP->nPPT          = 0;
			pTCP->pPPTData      = NULL;
			pTCP->pPPTDataFirst = NULL;
			pTCP->pTCCP         = pTemp;

			for (int nIndex = 0; nIndex < pJ2k->pImage->nCsiz; nIndex++)
			{
				pTCP->pTCCP[nIndex] = pJ2k->pDefaultTCP->pTCCP[nIndex];
			}
			pCodingParams->pTCP[pJ2k->nCurTileIndex].nFirst = 0;
		}
	}

	static void J2k_WriteSOD(J2kCodestream *pJ2k, void *pTileCoder)
	{
		TCD *pTCD = (TCD*)pTileCoder;
		CodingParams *pCodingParams = pJ2k->pCodingParams;
		CReader *pStream = pJ2k->pStreamIO;

		pStream->Write(J2K_MS_SOD, 2); // SOD

		if (pJ2k->nCurTileIndex == 0)
		{
			pJ2k->nSODStartPos =  pStream->Tell() + pJ2k->nPosCorrection;
		}

		// INDEX
		ImageInfo *pImageInfo = pJ2k->pImageInfo;
		if (pImageInfo && pImageInfo->nIndexOn)
		{
			pImageInfo->pTile[pJ2k->nCurTileIndex].nEndHeader =  pStream->Tell() + pJ2k->nPosCorrection - 1;
		}
		// INDEX

		TileCodingParams *pTCP = &pCodingParams->pTCP[pJ2k->nCurTileIndex];
		for (int nCurLayer = 0; nCurLayer < pTCP->nLayersCount; nCurLayer++)
		{
			pTCP->afRates[nCurLayer] -= pTCP->afRates[nCurLayer] ? (pJ2k->nSODStartPos / (pCodingParams->nYTilesCount * pCodingParams->nXTilesCount)) : 0;
		}
		if (pImageInfo)
		{
			pImageInfo->nPacketCount = 0;
		}

		int nTileDataLen = TCD_EncodeTile(pTCD, pJ2k->nCurTileIndex, (BYTE*)pStream->GetOwner() + pStream->Tell(), pStream->GetLeftSize() - 2, pImageInfo);

		// Записываем поле Psot в маркере SOT
		int nTotalLen =  pStream->Tell() + nTileDataLen - pJ2k->nSOTStartPos;
		pStream->Seek(pJ2k->nSOTStartPos + 6);
		pStream->Write(nTotalLen, 4); // Psot
		pStream->Seek(pJ2k->nSOTStartPos + nTotalLen);
	}

	static void J2k_ReadSOD(J2kCodestream *pJ2k)
	{
		int nTruncate = 0;

		CReader *pStream = pJ2k->pStreamIO;
		int nCurTileIndex = pJ2k->nCurTileIndex;

        int nLen = (std::min)(pJ2k->nSizeOfTile - pStream->Tell(), pStream->GetLeftSize() + 1);

		if (nLen == pStream->GetLeftSize() + 1)
		{
			nTruncate = 1; // Обрезанный codestream
		}

		unsigned char *pData = (unsigned char *)Malloc((pJ2k->pTileLen[nCurTileIndex] + nLen) * sizeof(unsigned char));
		if (!pData)
		{
			pJ2k->pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
			return;
		}

		for (int nIndex = 0; nIndex < pJ2k->pTileLen[nCurTileIndex]; nIndex++)
		{
			pData[nIndex] = pJ2k->ppTileData[nCurTileIndex][nIndex];
		}

		unsigned char *pDataPointer = pData + pJ2k->pTileLen[nCurTileIndex];
		//for ( int nIndex = 0; nIndex < nLen; nIndex++ ) 
		//{
		//	pDataPointer[nIndex] = pStream->Read( 1 ); // Считываем закодированные данные текущего тайла
		//}

		pStream->Read(pDataPointer, nLen);

		pJ2k->pTileLen[nCurTileIndex] += nLen;
		Free(pJ2k->ppTileData[nCurTileIndex]);
		pJ2k->ppTileData[nCurTileIndex] = pData;

		if (!nTruncate)
		{
			pJ2k->nState = j2kstateTPHSOT;
		}
		else
		{
			pJ2k->nState = j2kstateNEOC;
		}
	}

	static void J2k_WriteRGN(J2kCodestream *pJ2k, int nCurComponent, int nTileIndex)
	{
		CodingParams *pCodingParams = pJ2k->pCodingParams;
		TileCodingParams *pTCP = &pCodingParams->pTCP[nTileIndex];
		CReader *pStream = pJ2k->pStreamIO;

		int nComponentsCount = pJ2k->pImage->nCsiz;

		pStream->Write(J2K_MS_RGN, 2); // RGN
		pStream->Write(nComponentsCount <= 256 ? 5 : 6, 2); // Lrgn
		pStream->Write(nCurComponent, nComponentsCount <= 256 ? 1 : 2); // Crgn
		pStream->Write(0, 1); // Srgn
		pStream->Write(pTCP->pTCCP[nCurComponent].nROIShift, 1); // SPrgn
	}

	static void J2k_ReadRGN(J2kCodestream *pJ2k)
	{
		CodingParams *pCodingParams = pJ2k->pCodingParams;
		TileCodingParams *pTCP = pJ2k->nState == j2kstateTPH ? &pCodingParams->pTCP[pJ2k->nCurTileIndex] : pJ2k->pDefaultTCP;
		CReader *pStream = pJ2k->pStreamIO;

		int nComponentsCount = pJ2k->pImage->nCsiz;

		int nLen          = pStream->Read(2); // Lrgn
		int nCurComponent = pStream->Read(nComponentsCount <= 256 ? 1 : 2); // Crgn
		int nROIStyle     = pStream->Read(1); // Srgn
		// TO DO: Вставить проверку nROIStyle == 0
		pTCP->pTCCP[nCurComponent].nROIShift = pStream->Read(1); // SPrgn 
	}

	static void J2k_WriteEOC(J2kCodestream *pJ2k)
	{
		CReader *pStream = pJ2k->pStreamIO;
		pStream->Write(J2K_MS_EOC, 2); // EOC
	}

	static void J2k_ReadEOC(J2kCodestream *pJ2k)
	{
#ifndef NO_PACKETS_DECODING  
		TCD *pTCD = TCD_Create(pJ2k->pCodecInfo);
		TCD_MallocDecode(pTCD, pJ2k->pImage, pJ2k->pCodingParams);

		if (pTCD->pCodecInfo->nErrorCode == JP2_ERROR_NO_ERROR)
		{
			for (int nIndex = 0; nIndex < pJ2k->pCodingParams->nTileIndexSize; nIndex++)
			{
				int nTileIndex = pJ2k->pCodingParams->pTileIndex[nIndex];
				TCD_DecodeTile(pTCD, pJ2k->ppTileData[nTileIndex], pJ2k->pTileLen[nTileIndex], nTileIndex);
				Free(pJ2k->ppTileData[nTileIndex]);
				pJ2k->ppTileData[nTileIndex] = NULL;
			}
			TCD_FreeDecode(pTCD);
		}
		else
		{
			for (int nIndex = 0; nIndex < pJ2k->pCodingParams->nTileIndexSize; nIndex++)
			{
				int nTileIndex = pJ2k->pCodingParams->pTileIndex[nIndex];
				Free(pJ2k->ppTileData[nTileIndex]);
				pJ2k->ppTileData[nTileIndex] = NULL;
			}
		}
		TCD_Destroy(pTCD);
#else 
		for ( int nIndex = 0; nIndex < j2k->cp->tileno_size; nIndex++ ) 
		{
			int nTileIndex = pJ2k->pCodingParams->pTileIndex[nIndex];
			Free( pJ2k->ppTileData[nTileIndex] );
			pJ2k->ppTileData[nTileIndex] = NULL;
		}
#endif

		pJ2k->nState = j2kstateMT;
	}
	static void J2k_ReadUNK(J2kCodestream *pJ2k)
	{
		// Предполагаем, что неизвестный маркер не пустой, а, значит,
		// у него следующие два байта - длина маркера.
		CReader *pStream = pJ2k->pStreamIO;
		int nLen = pStream->Read(2);

		if (nLen > 0)
			pStream->Read(nLen - 2);

		Event_Message(EVT_WARNING, "Unknown marker\n");
	}




	//-------------------------------------------------------------------------------------------------------------------------------
	// Таблица связывающая прочтенный маркер - состояние декодера - действие(функция) декодера при прочтении данного маркера
	typedef struct TDecoderMSTableEntry
	{
		int    nID;     // Значение маркера
		int    nStates; // Состояние декодера, когда появляется данный маркер
		void(*pHandler) (J2kCodestream *pJ2k); // Действие связанное с данным маркером
	} DecoderMSTableEntry;

	DecoderMSTableEntry c_aoJ2k_DecoderMSTable[] =
	{
		{ J2K_MS_SOC, j2kstateMHSOC, J2k_ReadSOC },
		{ J2K_MS_SOT, j2kstateMH | j2kstateTPHSOT, J2k_ReadSOT },
		{ J2K_MS_SOD, j2kstateTPH, J2k_ReadSOD },
		{ J2K_MS_EOC, j2kstateTPHSOT, J2k_ReadEOC },
		{ J2K_MS_SIZ, j2kstateMHSIZ, J2k_ReadSIZ },
		{ J2K_MS_COD, j2kstateMH | j2kstateTPH, J2k_ReadCOD },
		{ J2K_MS_COC, j2kstateMH | j2kstateTPH, J2k_ReadCOC },
		{ J2K_MS_RGN, j2kstateMH | j2kstateTPH, J2k_ReadRGN },
		{ J2K_MS_QCD, j2kstateMH | j2kstateTPH, J2k_ReadQCD },
		{ J2K_MS_QCC, j2kstateMH | j2kstateTPH, J2k_ReadQCC },
		{ J2K_MS_POC, j2kstateMH | j2kstateMH, J2k_ReadPOC },
		{ J2K_MS_TLM, j2kstateMH, J2k_ReadTLM },
		{ J2K_MS_PLM, j2kstateMH, J2k_ReadPLM },
		{ J2K_MS_PLT, j2kstateTPH, J2k_ReadPLT },
		{ J2K_MS_PPM, j2kstateMH, J2k_ReadPPM },
		{ J2K_MS_PPT, j2kstateTPH, J2k_ReadPPT },
		{ J2K_MS_SOP, 0, 0 },
		{ J2K_MS_CRG, j2kstateMH, J2k_ReadCRG },
		{ J2K_MS_COM, j2kstateMH | j2kstateTPH, J2k_ReadCOM },
		{ 0, j2kstateMH | j2kstateTPH, J2k_ReadUNK }
		//////////////////////////////////////////////////////////////////////////////////
		//,{J2K_MS_EPC, j2kstateMH	 | j2kstateMH,		J2k_ReadEPC},
		//{J2K_MS_EPB, j2kstateMH  | j2kstateMH,		j2k_read_epb},
		//{J2K_MS_ESD, j2kstateMH  | j2kstateMH,		j2k_read_esd},
		//{J2K_MS_RED, j2kstateMH  | j2kstateMH,		j2k_read_red},
		//{J2K_MS_SEC, j2kstateMH/*J2K_DEC_STATE_MH*/,j2k_read_sec},
		//{J2K_MS_INSEC,		0,						j2k_read_insec},
		//{J2K_MS_UNK, j2kstateMH | J2K_STATE_TPH,	0}

	};

	static DecoderMSTableEntry *J2k_DeocderMSTableLookup(int nID)
	{
		DecoderMSTableEntry *pEntry = NULL;
		for (pEntry = c_aoJ2k_DecoderMSTable; pEntry->nID != 0; pEntry++)
		{
			if (pEntry->nID == nID)
			{
				break;
			}
		}
		return pEntry;
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// Декодирование потоков J2K / JPT
	//-------------------------------------------------------------------------------------------------------------------------------

	J2kCodestream* J2k_CreateDecompress(PCommon pCodecInfo)
	{
		J2kCodestream *pJ2k = (J2kCodestream*)Malloc(sizeof(J2kCodestream));
		if (pJ2k)
		{
			pJ2k->pCodecInfo  = pCodecInfo;
			pJ2k->pDefaultTCP = (TileCodingParams*)Malloc(sizeof(TileCodingParams));
			if (!pJ2k->pDefaultTCP)
			{
				pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
				Free(pJ2k);
				return NULL;
			}
		}
		else
		{
			pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
		}
		return pJ2k;
	}

	void  J2k_DestroyDecompress(J2kCodestream *pJ2k)
	{
		if (!pJ2k)return;

		Free(pJ2k->pTileLen);
		Free(pJ2k->ppTileData);

		if (pJ2k->pDefaultTCP != NULL)
		{
			TileCodingParams *pDefaultTCP = pJ2k->pDefaultTCP;

			Free(pDefaultTCP->pPPTDataFirst);
			Free(pJ2k->pDefaultTCP->pTCCP);
			Free(pJ2k->pDefaultTCP);
		}
		if (pJ2k->pCodingParams != NULL)
		{
			CodingParams *pCodingParams = pJ2k->pCodingParams;
			if (pCodingParams->pTCP != NULL)
			{
				for (int nIndex = 0; nIndex < pCodingParams->nXTilesCount * pCodingParams->nYTilesCount; nIndex++)
				{
					Free(pCodingParams->pTCP[nIndex].pPPTDataFirst);
					Free(pCodingParams->pTCP[nIndex].pTCCP);
				}
				Free(pCodingParams->pTCP);
			}
			Free(pCodingParams->pPPMDataFirst);
			Free(pCodingParams->pTileIndex);
			Free(pCodingParams->sComment);
			Free(pCodingParams);
		}

		Free(pJ2k);
	}

	void           J2k_SetupDecoder(J2kCodestream *pJ2k, DecoderParams *pParameters)
	{
		if (pJ2k && pParameters)
		{
			// Создаем и инициализируем структуру CodingParams
			CodingParams *pCodingParams = (CodingParams*)Malloc(sizeof(CodingParams));
			if (!pCodingParams)
			{
				pJ2k->pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
				return;
			}
			pCodingParams->nReduceFactor  = pParameters->nReduce;
			pCodingParams->nLayerFactor   = pParameters->nLayer;
			pCodingParams->eLimitDecoding = pParameters->eLimitDecoding;
			pJ2k->pCodingParams = pCodingParams;
		}
	}

	Image*         J2k_Decode(J2kCodestream *pJ2k, CReader *pStream)
	{
		PCommon pCodecInfo = pJ2k->pCodecInfo;

		pJ2k->pStreamIO = pStream;

		// Создаем пустую картинку
		Image *pImage = Image_CreateEmpty();
		if (!pImage)
		{
			pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
			return NULL;
		}
		pJ2k->pImage = pImage;

		pJ2k->nState = j2kstateMHSOC;

		for (;;)
		{
			int nID = pStream->Read(2);

			if (nID >> 8 != 0xff)
			{
				Image_Destroy(pImage);
				pImage = NULL;
				Event_Message(EVT_ERROR, "%.8x: expected a marker instead of %x\n", pStream->Tell() - 2, nID);
				return 0;
			}
			DecoderMSTableEntry *pEntry = J2k_DeocderMSTableLookup(nID);

			if (!(pJ2k->nState & pEntry->nStates))
			{
				Image_Destroy(pImage);
				pImage = NULL;
				Event_Message(EVT_ERROR, "%.8x: unexpected marker %x\n", pStream->Tell() - 2, nID);
				return 0;
			}

			// Проверяем ограничение на декодирование
			if (pEntry->nID == J2K_MS_SOT && pJ2k->pCodingParams->eLimitDecoding == declimLimitToMainHeader)
			{
				Event_Message(EVT_INFO, "Main Header decoded.\n");
				return pImage;
			}

			if (pEntry->pHandler)
			{
				(*pEntry->pHandler)(pJ2k);
			}
			else
			{
				Image_Destroy(pImage);
				pImage = NULL;
				Event_Message(EVT_ERROR, "Error handler occure");
				break;
			}
			if (pJ2k->nState == j2kstateMT)
			{
				pCodecInfo->nErrorCode = JP2_ERROR_NO_ERROR;
				break;
			}
			if (pJ2k->nState == j2kstateNEOC)
			{
				pCodecInfo->nErrorCode = JP2_ERROR_NO_ERROR;
				break;
			}
			// Проверяем не появилась ли ошибка
			if (JP2_ERROR_NO_ERROR != pCodecInfo->nErrorCode)
			{
				Image_Destroy(pImage);
				pImage = NULL;
				Event_Message(EVT_ERROR, "Error occure");
				return 0;
			}
		}
		if (pJ2k->nState == j2kstateNEOC)
		{
			J2k_ReadEOC(pJ2k);
		}

		if (pJ2k->nState != j2kstateMT)
		{
			Event_Message(EVT_WARNING, "Incomplete bitstream\n");
		}

		return pImage;
	}

	Image*         J2k_DecodeJptStream(J2kCodestream *pJ2k, CReader *pStream)
	{
		PCommon pCodecInfo = pJ2k->pCodecInfo;

		pJ2k->pStreamIO = pStream;

		// Создем пустую картинку
		Image *pImage = Image_CreateEmpty();
		if (!pImage)
		{
			pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
			return NULL;
		}

		pJ2k->nState = j2kstateMHSOC;

		// Инициализируем заголовок
		JPTMessageHeader oHeader;
		JPT_InitMessageHeader(&oHeader);
		// Читаем первый заголовок сообщения
		JPT_ReadMessageHeader(pCodecInfo, pStream, &oHeader);

		int nPosition =  pStream->Tell();
		if (oHeader.nClassId != 6) // 6 : Main header data-bin message
		{
			Image_Destroy(pImage);
			Event_Message(EVT_ERROR, "[JPT-stream] : Expecting Main header first [class_Id %d] !\n", oHeader.nClassId);
			return 0;
		}

		for (;;)
		{
			if (!pStream->GetLeftSize())
			{
				J2k_ReadEOC(pJ2k);
				return pImage;
			}

			if ((unsigned int)(pStream->Tell() - nPosition) == oHeader.nMessageLength)
			{
				JPT_ReadMessageHeader(pCodecInfo, pStream, &oHeader);
				nPosition =  pStream->Tell();
				if (oHeader.nClassId != 4) // 4 : Tile data-bin message
				{
					Image_Destroy(pImage);
					Event_Message(EVT_ERROR, "[JPT-stream] : Expecting Tile info !\n");
					return 0;
				}
			}

			int nId = pStream->Read(2);
			if (nId >> 8 != 0xff)
			{
				Image_Destroy(pImage);
				Event_Message(EVT_ERROR, "%.8x: expected a marker instead of %x\n", pStream->Tell() - 2, nId);
				return 0;
			}
			DecoderMSTableEntry *pEntry = J2k_DeocderMSTableLookup(nId);
			if (!(pJ2k->nState & pEntry->nStates))
			{
				Image_Destroy(pImage);
				Event_Message(EVT_ERROR, "%.8x: unexpected marker %x\n", pStream->Tell() - 2, nId);
				return 0;
			}
			if (pEntry->pHandler)
			{
				(*pEntry->pHandler)(pJ2k);
			}
			if (pJ2k->nState == j2kstateMT)
			{
				break;
			}
			if (pJ2k->nState == j2kstateNEOC)
			{
				break;
			}
		}
		if (pJ2k->nState == j2kstateNEOC)
		{
			J2k_ReadEOC(pJ2k);
		}

		if (pJ2k->nState != j2kstateMT)
		{
			Event_Message(EVT_WARNING, "Incomplete bitstream\n");
		}

		return pImage;
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// Кодирование в поток J2K
	//-------------------------------------------------------------------------------------------------------------------------------

	J2kCodestream* J2k_CreateCompress(PCommon pCodecInfo)
	{
		J2kCodestream *pJ2k = (J2kCodestream*)Malloc(sizeof(J2kCodestream));
		if (pJ2k)
		{
			pJ2k->pCodecInfo = pCodecInfo;
		}
		else
		{
			pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
		}
		return pJ2k;
	}

	void           J2k_DestroyCompress(J2kCodestream *pJ2k)
	{
		if (!pJ2k)
			return;

		if (pJ2k->pImageInfo != NULL)
		{
			ImageInfo *pImageInfo = pJ2k->pImageInfo;
			if (pImageInfo->nIndexOn && pJ2k->pCodingParams)
			{
				CodingParams *pCodingParams = pJ2k->pCodingParams;
				for (int nTileIndex = 0; nTileIndex < pCodingParams->nXTilesCount * pCodingParams->nYTilesCount; nTileIndex++)
				{
					TileInfo *pTileInfo = &pImageInfo->pTile[nTileIndex];
					Free(pTileInfo->pdThreshold);
					Free(pTileInfo->pPacket);
				}
				Free(pImageInfo->pTile);
			}
			Free(pImageInfo);
		}
		if (pJ2k->pCodingParams != NULL)
		{
			CodingParams *pCodingParams = pJ2k->pCodingParams;

			Free(pCodingParams->sComment);
			Free(pCodingParams->pMatrix);

			for (int nTileIndex = 0; nTileIndex < pCodingParams->nXTilesCount * pCodingParams->nYTilesCount; nTileIndex++)
			{
				Free(pCodingParams->pTCP[nTileIndex].pTCCP);
			}
			Free(pCodingParams->pTCP);
			Free(pCodingParams);
		}

		Free(pJ2k);
	}

	void           J2k_SetupEncoder(J2kCodestream *pJ2k, EncoderParams *pParameters, Image *pImage)
	{
		if (!pJ2k || !pParameters || !pImage)
		{
			return;
		}

		// Создаем и инициализируем структуру Coding parameters
		CodingParams *pCodingParams = (CodingParams*)Malloc(sizeof(CodingParams));
		if (!pCodingParams)
		{
			pJ2k->pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
			return;
		}
		pJ2k->pCodingParams = pCodingParams;

		pCodingParams->nXTilesCount = 1;
		pCodingParams->nYTilesCount = 1;

		// Копируем заданные параметры 

		pCodingParams->nDistoAlloc   = pParameters->nDistoAlloc;
		pCodingParams->nFixedAlloc   = pParameters->nFixedAlloc;
		pCodingParams->nFixedQuality = pParameters->nFixedQuality;

		// Fixed_quality
		if (pParameters->pMatrix)
		{
			size_t array_size = pParameters->nLayersCount * pParameters->nResolutionsCount * 3 * sizeof(int);
			pCodingParams->pMatrix = (int *)Malloc(array_size);
			if (!pCodingParams->pMatrix)
			{
				Free(pCodingParams);
				pJ2k->pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
				return;
			}
			memcpy(pCodingParams->pMatrix, pParameters->pMatrix, array_size);
		}

		// Создаем ли файл индексации?
		pCodingParams->nIndexOn = pParameters->nIndexOn;
		if (pCodingParams->nIndexOn)
		{
			pJ2k->pImageInfo = (ImageInfo*)Malloc(sizeof(ImageInfo));
			if (!pJ2k->pImageInfo)
			{
				if (pCodingParams->pMatrix)
					Free(pCodingParams->pMatrix);
				Free(pCodingParams);
				pJ2k->pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
				return;
			}
		}

		// Tiles
		pCodingParams->nXTsiz = pParameters->nXTsiz;
		pCodingParams->nYTsiz = pParameters->nYTsiz;

		// Tile offset
		pCodingParams->nXTOsiz = pParameters->nXTOsiz;
		pCodingParams->nYTOsiz = pParameters->nYTOsiz;

		// Comment
		if (pParameters->sComment)
		{
			pCodingParams->sComment = (char*)Malloc(strlen(pParameters->sComment) + 1);
			if (pCodingParams->sComment)
			{
				strcpy(pCodingParams->sComment, pParameters->sComment);
			}
		}

		// Вычисляем другие параметры кодирования

		if (pParameters->bTileSizeOn)
		{
			pCodingParams->nXTilesCount = CeilDiv(pImage->nXsiz - pCodingParams->nXTOsiz, pCodingParams->nXTsiz);
			pCodingParams->nYTilesCount = CeilDiv(pImage->nYsiz - pCodingParams->nYTOsiz, pCodingParams->nYTsiz);
		}
		else
		{
			pCodingParams->nXTsiz = pImage->nXsiz - pCodingParams->nXTOsiz;
			pCodingParams->nYTsiz = pImage->nYsiz - pCodingParams->nYTOsiz;
		}


		pCodingParams->pTCP = (TileCodingParams *)Malloc(pCodingParams->nXTilesCount * pCodingParams->nYTilesCount * sizeof(TileCodingParams));
		if (!pCodingParams->pTCP)
		{
			Free(pCodingParams->pMatrix);
			Free(pJ2k->pImageInfo);
			Free(pCodingParams);

			pJ2k->pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
			return;
		}

		for (int nTileIndex = 0; nTileIndex < pCodingParams->nXTilesCount * pCodingParams->nYTilesCount; nTileIndex++)
		{
			TileCodingParams *pTCP = &pCodingParams->pTCP[nTileIndex];
			pTCP->nLayersCount = pParameters->nLayersCount;
			for (int nLayerIndex = 0; nLayerIndex < pTCP->nLayersCount; nLayerIndex++)
			{
				if (pCodingParams->nFixedQuality) // Add fixed quality
				{
					pTCP->afDistoRatio[nLayerIndex] = pParameters->afDistoratio[nLayerIndex];
				}
				else
				{
					pTCP->afRates[nLayerIndex]      = pParameters->afRates[nLayerIndex];
				}
			}
			pTCP->nCodingStyle = pParameters->nCodingStyle;
			pTCP->eProgOrder   = pParameters->eProgOrder;
			pTCP->nMCT = pImage->nCsiz == 3 ? 1 : 0;

			int nPOCsCountTile = 0;
			pTCP->nUsePOC = 0;

			if (pParameters->nPOCsCount)
			{
				pTCP->nUsePOC = 1;
				for (int nPOCIndex = 0; nPOCIndex < pParameters->nPOCsCount; nPOCIndex++)
				{
					if ((nTileIndex == pParameters->aoPOC[nPOCIndex].nTile - 1) || (pParameters->aoPOC[nPOCIndex].nTile == -1))
					{
						POC *pTCP_POC = &pTCP->aoPOC[nPOCsCountTile];
						pTCP_POC->nRSpoc  = pParameters->aoPOC[nPOCsCountTile].nRSpoc;
						pTCP_POC->nCSpoc  = pParameters->aoPOC[nPOCsCountTile].nCSpoc;
						pTCP_POC->nLYEpoc = pParameters->aoPOC[nPOCsCountTile].nLYEpoc;
						pTCP_POC->nREpoc  = pParameters->aoPOC[nPOCsCountTile].nREpoc;
						pTCP_POC->nCEpoc  = pParameters->aoPOC[nPOCsCountTile].nCEpoc;
						pTCP_POC->ePpoc   = pParameters->aoPOC[nPOCsCountTile].ePpoc;
						pTCP_POC->nTile   = pParameters->aoPOC[nPOCsCountTile].nTile;
						nPOCsCountTile++;
					}
				}
			}
			pTCP->nPOCsCount = nPOCsCountTile;

			pTCP->pTCCP = (TileCompCodingParams *)Malloc(pImage->nCsiz * sizeof(TileCompCodingParams));
			if (!pTCP->pTCCP)
			{
				for (int nI = 0; nI < nTileIndex; nI++)
				{
					TileCodingParams *pDeleteTCP = &pCodingParams->pTCP[nI];
					Free(pDeleteTCP->pTCCP);
				}
				Free(pCodingParams->pMatrix);
				Free(pJ2k->pImageInfo);

				Free(pCodingParams->pTCP);
				Free(pCodingParams);
				pJ2k->pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
				return;
			}

			for (int nComponentIndex = 0; nComponentIndex < pImage->nCsiz; nComponentIndex++)
			{
				TileCompCodingParams *pTCCP = &pTCP->pTCCP[nComponentIndex];
				pTCCP->nCodingStyle      = pParameters->nCodingStyle & 0x01;	// 0 => одна область || 1 => призвольное количество областей
				pTCCP->nResolutionsCount = pParameters->nResolutionsCount;
				pTCCP->nCodeBlockWidth   = FloorLog2(pParameters->nCodeBlockWidthInit);
				pTCCP->nCodeBlockHeight  = FloorLog2(pParameters->nCodeBlockHeightInit);
				pTCCP->nCodeBlockStyle   = pParameters->nCodeBlockStyle;
				pTCCP->nTransformID      = pParameters->nTransformId ? 0 : 1;
				pTCCP->nQuantStyle       = pParameters->nTransformId ? J2K_CCP_QNTSTY_SEQNT : J2K_CCP_QNTSTY_NOQNT;
				pTCCP->nGuardBitsCount   = 2;

				if (nComponentIndex == pParameters->nROIComponentIndex)
				{
					pTCCP->nROIShift = pParameters->nROIShift;
				}
				else
				{
					pTCCP->nROIShift = 0;
				}

				if (pParameters->nCodingStyle & J2K_CCP_CSTY_PRT)
				{
					int nPrecinctIndex = 0;
					for (int nLevelIndex = pTCCP->nResolutionsCount - 1; nLevelIndex >= 0; nLevelIndex--)
					{
						if (nPrecinctIndex < pParameters->nPrecinctCount)
						{
							if (pParameters->anPrecinctWidthInit[nPrecinctIndex] < 1)
							{
								pTCCP->anPrecinctWidth[nLevelIndex] = 1;
							}
							else
							{
								pTCCP->anPrecinctWidth[nLevelIndex] = FloorLog2(pParameters->anPrecinctWidthInit[nPrecinctIndex]);
							}
							if (pParameters->anPrecinctHeightInit[nPrecinctIndex] < 1)
							{
								pTCCP->anPrecinctHeight[nLevelIndex] = 1;
							}
							else
							{
								pTCCP->anPrecinctHeight[nLevelIndex] = FloorLog2(pParameters->anPrecinctHeightInit[nPrecinctIndex]);
							}
						}
						else
						{
							int nPrecinctCount  = pParameters->nPrecinctCount;
							int nPrecinctWidth  = pParameters->anPrecinctWidthInit[nPrecinctCount - 1] >> (nPrecinctIndex - (nPrecinctCount - 1));
							int nPrecinctHeight = pParameters->anPrecinctHeightInit[nPrecinctCount - 1] >> (nPrecinctIndex - (nPrecinctCount - 1));
							if (nPrecinctWidth < 1)
							{
								pTCCP->anPrecinctWidth[nLevelIndex] = 1;
							}
							else
							{
								pTCCP->anPrecinctWidth[nLevelIndex] = FloorLog2(nPrecinctWidth);
							}
							if (nPrecinctHeight < 1)
							{
								pTCCP->anPrecinctHeight[nLevelIndex] = 1;
							}
							else
							{
								pTCCP->anPrecinctHeight[nLevelIndex] = FloorLog2(nPrecinctHeight);
							}
						}
						nPrecinctIndex++;
					}
				}
				else
				{
					for (int nLevelIndex = 0; nLevelIndex < pTCCP->nResolutionsCount; nLevelIndex++)
					{
						pTCCP->anPrecinctWidth[nLevelIndex]  = 15;
						pTCCP->anPrecinctHeight[nLevelIndex] = 15;
					}
				}

				DWT_CalcExplicitStepsizes(pTCCP, pImage->pComponents[nComponentIndex].nPrecision);
			}
		}
	}

	static int     J2k_CreateIndex(J2kCodestream *pJ2k, CReader *pStream, ImageInfo *pImageInfo, char *sIndex)
	{
		double dTotalDisto = 0;

		pImageInfo->nCodestreamSize =  pStream->Tell() + pJ2k->nPosCorrection;

		FILE* pFile = fopen(sIndex, "w");
		if (!pFile)
		{
			Event_Message(EVT_ERROR, "failed to open %s for writing\n", sIndex);
			return 0;
		}

		fprintf(pFile, "%d %d\n", pImageInfo->nImageWidth, pImageInfo->nImageHeight);
		fprintf(pFile, "%d\n", pImageInfo->eProgOrder);
		fprintf(pFile, "%d %d\n", pImageInfo->nXTsiz, pImageInfo->nYTsiz);
		fprintf(pFile, "%d %d\n", pImageInfo->nXTilesCount, pImageInfo->nYTilesCount);
		fprintf(pFile, "%d\n", pImageInfo->nCompCount);
		fprintf(pFile, "%d\n", pImageInfo->nLayersCount);
		fprintf(pFile, "%d\n", pImageInfo->nDecompCount);

		for (int nResolutionIndex = pImageInfo->nDecompCount; nResolutionIndex >= 0; nResolutionIndex--)
		{
			fprintf(pFile, "[%d,%d] ", (1 << pImageInfo->pTile[0].anPrecinctWidth[nResolutionIndex]), (1 << pImageInfo->pTile[0].anPrecinctWidth[nResolutionIndex]));
		}
		fprintf(pFile, "\n");
		fprintf(pFile, "%d\n", pImageInfo->nMainHeadEnd);
		fprintf(pFile, "%d\n", pImageInfo->nCodestreamSize);

		for (int nTileIndex = 0; nTileIndex < pImageInfo->nXTilesCount * pImageInfo->nYTilesCount; nTileIndex++)
		{
			fprintf(pFile, "%4d %9d %9d %9d %9e %9d %9e\n", pImageInfo->pTile[nTileIndex].nTileCount, pImageInfo->pTile[nTileIndex].nStartPos, pImageInfo->pTile[nTileIndex].nEndHeader, pImageInfo->pTile[nTileIndex].nEndPos, pImageInfo->pTile[nTileIndex].dDistoTile, pImageInfo->pTile[nTileIndex].nNBPix, pImageInfo->pTile[nTileIndex].dDistoTile / pImageInfo->pTile[nTileIndex].nNBPix);
		}

		for (int nTileIndex = 0; nTileIndex < pImageInfo->nXTilesCount * pImageInfo->nYTilesCount; nTileIndex++)
		{
			int nStartPos = 0, nEndPos = 0;
			double dDisto = 0;
			int nPackNB = 0;

			if (pImageInfo->eProgOrder == poLRCP)
			{
				for (int nLayerIndex = 0; nLayerIndex < pImageInfo->nLayersCount; nLayerIndex++)
				{
					for (int nResolutionIndex = 0; nResolutionIndex < pImageInfo->nDecompCount + 1; nResolutionIndex++)
					{
						for (int nComponentIndex = 0; nComponentIndex < pImageInfo->nCompCount; nComponentIndex++)
						{
							int nPrecinctMax = pImageInfo->pTile[nTileIndex].anXPrecinctCount[nResolutionIndex] * pImageInfo->pTile[nTileIndex].anYPrecinctCount[nResolutionIndex];
							for (int nPrecinctIndex = 0; nPrecinctIndex < nPrecinctMax; nPrecinctIndex++)
							{
								nStartPos = pImageInfo->pTile[nTileIndex].pPacket[nPackNB].nStartPos;
								nEndPos   = pImageInfo->pTile[nTileIndex].pPacket[nPackNB].nEndPos;
								dDisto    = pImageInfo->pTile[nTileIndex].pPacket[nPackNB].dDisto;
								fprintf(pFile, "%4d %6d %7d %5d %6d %6d %9d %9d %8e\n", nPackNB, nTileIndex, nLayerIndex, nResolutionIndex, nComponentIndex, nPrecinctIndex, nStartPos, nEndPos, dDisto);
								dTotalDisto += dDisto;
								nPackNB++;
							}
						}
					}
				}
			}
			else if (pImageInfo->eProgOrder == poRLCP)
			{
				for (int nResolutionIndex = 0; nResolutionIndex < pImageInfo->nDecompCount + 1; nResolutionIndex++)
				{
					for (int nLayerIndex = 0; nLayerIndex < pImageInfo->nLayersCount; nLayerIndex++)
					{
						for (int nComponentIndex = 0; nComponentIndex < pImageInfo->nCompCount; nComponentIndex++)
						{
							int nPrecinctMax = pImageInfo->pTile[nTileIndex].anXPrecinctCount[nResolutionIndex] * pImageInfo->pTile[nTileIndex].anYPrecinctCount[nResolutionIndex];
							for (int nPrecinctIndex = 0; nPrecinctIndex < nPrecinctMax; nPrecinctIndex++)
							{
								nStartPos = pImageInfo->pTile[nTileIndex].pPacket[nPackNB].nStartPos;
								nEndPos   = pImageInfo->pTile[nTileIndex].pPacket[nPackNB].nEndPos;
								dDisto    = pImageInfo->pTile[nTileIndex].pPacket[nPackNB].dDisto;
								fprintf(pFile, "%4d %6d %5d %7d %6d %6d %9d %9d %8e\n", nPackNB, nTileIndex, nResolutionIndex, nLayerIndex, nComponentIndex, nPrecinctIndex, nStartPos, nEndPos, dDisto);
								dTotalDisto += dDisto;
								nPackNB++;
							}
						}
					}
				}
			}
			else if (pImageInfo->eProgOrder == poRPCL)
			{
				for (int nResolutionIndex = 0; nResolutionIndex < pImageInfo->nDecompCount + 1; nResolutionIndex++)
				{
					// Предполагаем, что значения XRsiz, YRsiz одинаковые для всех компонент
					int nX0 = pImageInfo->nXTOsiz + nTileIndex - (int)floor((float)nTileIndex / (float)pImageInfo->nXTilesCount) * pImageInfo->nXTilesCount * pImageInfo->nXTsiz;
					int nY0 = pImageInfo->nXTOsiz + (int)floor((float)nTileIndex / (float)pImageInfo->nXTilesCount) * pImageInfo->nYTsiz;
					int nX1 = nX0 + pImageInfo->nXTsiz;
					int nY1 = nY0 + pImageInfo->nYTsiz;
					for (int nY = nY0; nY < nY1; nY++)
					{
						for (int nX = nX0; nX < nX1; nX++)
						{
							for (int nComponentIndex = 0; nComponentIndex < pImageInfo->nCompCount; nComponentIndex++)
							{
								int nPrecinctMax = pImageInfo->pTile[nTileIndex].anXPrecinctCount[nResolutionIndex] * pImageInfo->pTile[nTileIndex].anYPrecinctCount[nResolutionIndex];
								for (int nPrecinctIndex = 0; nPrecinctIndex < nPrecinctMax; nPrecinctIndex++)
								{
									int nXPrecinctCount = pImageInfo->pTile[nTileIndex].anXPrecinctCount[nResolutionIndex];
									int nPrecinctX = (int)pow(double(2), pImageInfo->pTile[nTileIndex].anPrecinctWidth[nResolutionIndex] + pImageInfo->nDecompCount - nResolutionIndex);
									int nPrecinctY = (int)pow(double(2), pImageInfo->pTile[nTileIndex].anPrecinctHeight[nResolutionIndex] + pImageInfo->nDecompCount - nResolutionIndex);
									int nPrecinctXIndex = nPrecinctIndex - (int)floor((float)nPrecinctIndex / (float)nXPrecinctCount) * nXPrecinctCount;
									int nPrecinctYIndex = (int)floor((float)nPrecinctIndex / (float)nXPrecinctCount);
									if (nPrecinctYIndex * nPrecinctY == nY)
									{
										if (nPrecinctXIndex * nPrecinctX == nX)
										{
											for (int nLayerIndex = 0; nLayerIndex < pImageInfo->nLayersCount; nLayerIndex++)
											{
												nStartPos = pImageInfo->pTile[nTileIndex].pPacket[nPackNB].nStartPos;
												nEndPos   = pImageInfo->pTile[nTileIndex].pPacket[nPackNB].nEndPos;
												dDisto    = pImageInfo->pTile[nTileIndex].pPacket[nPackNB].dDisto;
												fprintf(pFile, "%4d %6d %5d %6d %6d %7d %9d %9d %8e\n", nPackNB, nTileIndex, nResolutionIndex, nPrecinctIndex, nComponentIndex, nLayerIndex, nStartPos, nEndPos, dDisto);
												dTotalDisto += dDisto;
												nPackNB++;
											}
										}
									}
								}
							}
						}
					}
				}
			}
			else if (pImageInfo->eProgOrder == poPCRL)
			{
				// Предполагаем, что значения XRsiz, YRsiz одинаковые для всех компонент
				int nX0 = pImageInfo->nXTOsiz + nTileIndex - (int)floor((float)nTileIndex / (float)pImageInfo->nXTilesCount) * pImageInfo->nXTilesCount * pImageInfo->nXTsiz;
				int nY0 = pImageInfo->nXTOsiz + (int)floor((float)nTileIndex / (float)pImageInfo->nXTilesCount) * pImageInfo->nYTsiz;
				int nX1 = nX0 + pImageInfo->nXTsiz;
				int nY1 = nY0 + pImageInfo->nYTsiz;
				for (int nY = nY0; nY < nY1; nY++)
				{
					for (int nX = nX0; nX < nX1; nX++)
					{
						for (int nComponentIndex = 0; nComponentIndex < pImageInfo->nCompCount; nComponentIndex++)
						{
							for (int nResolutionIndex = 0; nResolutionIndex < pImageInfo->nDecompCount + 1; nResolutionIndex++)
							{
								int nPrecinctMax = pImageInfo->pTile[nTileIndex].anXPrecinctCount[nResolutionIndex] * pImageInfo->pTile[nTileIndex].anYPrecinctCount[nResolutionIndex];
								for (int nPrecinctIndex = 0; nPrecinctIndex < nPrecinctMax; nPrecinctIndex++)
								{
									int nXPrecinctCount = pImageInfo->pTile[nTileIndex].anXPrecinctCount[nResolutionIndex];
									int nPrecinctX = (int)pow(double(2), pImageInfo->pTile[nTileIndex].anPrecinctWidth[nResolutionIndex] + pImageInfo->nDecompCount - nResolutionIndex);
									int nPrecinctY = (int)pow(double(2), pImageInfo->pTile[nTileIndex].anPrecinctHeight[nResolutionIndex] + pImageInfo->nDecompCount - nResolutionIndex);
									int nPrecinctXIndex = nPrecinctIndex - (int)floor((float)nPrecinctIndex / (float)nXPrecinctCount) * nXPrecinctCount;
									int nPrecinctYIndex = (int)floor((float)nPrecinctIndex / (float)nXPrecinctCount);
									if (nPrecinctYIndex * nPrecinctY == nY)
									{
										if (nPrecinctXIndex * nPrecinctX == nX)
										{
											for (int nLayerIndex = 0; nLayerIndex < pImageInfo->nLayersCount; nLayerIndex++)
											{
												nStartPos = pImageInfo->pTile[nTileIndex].pPacket[nPackNB].nStartPos;
												nEndPos   = pImageInfo->pTile[nTileIndex].pPacket[nPackNB].nEndPos;
												dDisto    = pImageInfo->pTile[nTileIndex].pPacket[nPackNB].dDisto;
												fprintf(pFile, "%4d %6d %6d %6d %5d %7d %9d %9d %8e\n", nPackNB, nTileIndex, nPrecinctIndex, nComponentIndex, nResolutionIndex, nLayerIndex, nStartPos, nEndPos, dDisto);
												dTotalDisto += dDisto;
												nPackNB++;
											}
										}
									}
								}
							}
						}
					}
				}
			}
			else
			{
				for (int nComponentIndex = 0; nComponentIndex < pImageInfo->nCompCount; nComponentIndex++)
				{
					// Предполагаем, что значения XRsiz, YRsiz одинаковые для всех компонент
					int nX0 = pImageInfo->nXTOsiz + nTileIndex - (int)floor((float)nTileIndex / (float)pImageInfo->nXTilesCount) * pImageInfo->nXTilesCount * pImageInfo->nXTsiz;
					int nY0 = pImageInfo->nXTOsiz + (int)floor((float)nTileIndex / (float)pImageInfo->nXTilesCount) * pImageInfo->nYTsiz;
					int nX1 = nX0 + pImageInfo->nXTsiz;
					int nY1 = nY0 + pImageInfo->nYTsiz;
					for (int nY = nY0; nY < nY1; nY++)
					{
						for (int nX = nX0; nX < nX1; nX++)
						{
							for (int nResolutionIndex = 0; nResolutionIndex < pImageInfo->nDecompCount + 1; nResolutionIndex++)
							{
								int nPrecinctMax = pImageInfo->pTile[nTileIndex].anXPrecinctCount[nResolutionIndex] * pImageInfo->pTile[nTileIndex].anYPrecinctCount[nResolutionIndex];
								for (int nPrecinctIndex = 0; nPrecinctIndex < nPrecinctMax; nPrecinctIndex++)
								{
									int nXPrecinctCount = pImageInfo->pTile[nTileIndex].anXPrecinctCount[nResolutionIndex];
									int nPrecinctX = (int)pow(double(2), pImageInfo->pTile[nTileIndex].anPrecinctWidth[nResolutionIndex] + pImageInfo->nDecompCount - nResolutionIndex);
									int nPrecinctY = (int)pow(double(2), pImageInfo->pTile[nTileIndex].anPrecinctHeight[nResolutionIndex] + pImageInfo->nDecompCount - nResolutionIndex);
									int nPrecinctXIndex = nPrecinctIndex - (int)floor((float)nPrecinctIndex / (float)nXPrecinctCount) * nXPrecinctCount;
									int nPrecinctYIndex = (int)floor((float)nPrecinctIndex / (float)nXPrecinctCount);
									if (nPrecinctYIndex * nPrecinctY == nY)
									{
										if (nPrecinctXIndex * nPrecinctX == nX)
										{
											for (int nLayerIndex = 0; nLayerIndex < pImageInfo->nLayersCount; nLayerIndex++)
											{
												nStartPos = pImageInfo->pTile[nTileIndex].pPacket[nPackNB].nStartPos;
												nEndPos   = pImageInfo->pTile[nTileIndex].pPacket[nPackNB].nEndPos;
												dDisto    = pImageInfo->pTile[nTileIndex].pPacket[nPackNB].dDisto;
												fprintf(pFile, "%4d %6d %6d %6d %5d %7d %9d %9d %8e\n", nPackNB, nTileIndex, nComponentIndex, nPrecinctIndex, nResolutionIndex, nLayerIndex, nStartPos, nEndPos, dDisto);
												dTotalDisto += dDisto;
												nPackNB++;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}

		fprintf(pFile, "%8e\n", pImageInfo->dDistoMax);
		fprintf(pFile, "%.8e\n", dTotalDisto);
		fclose(pFile);

		return 1;
	}

	bool           J2k_Encode(J2kCodestream *pJ2k, CReader *pStream, Image *pImage, char *sIndex)
	{
		pJ2k->pStreamIO = pStream;
		pJ2k->pImage    = pImage;

		CodingParams *pCodingParams = pJ2k->pCodingParams;

#ifdef _DEBUG
		FILE *pFile;
		pFile = fopen("E:\\Jpeg2000CodingParams.xml", "wb");
		if (pFile)
		{
			J2k_DumpCodingParams(pFile, pImage, pCodingParams);
			fclose(pFile);
		}
#endif

		// Индексация
		ImageInfo *pImageInfo = pJ2k->pImageInfo;
		if (pImageInfo && pCodingParams->nIndexOn)
		{
			pImageInfo->nIndexOn     = pCodingParams->nIndexOn;
			pImageInfo->pTile        = (TileInfo *)Malloc(pCodingParams->nXTilesCount * pCodingParams->nYTilesCount * sizeof(TileInfo));
			pImageInfo->nImageWidth  = pImage->nXsiz - pImage->nXOsiz;
			pImageInfo->nImageHeight = pImage->nYsiz - pImage->nYOsiz;
			pImageInfo->eProgOrder   = (&pCodingParams->pTCP[0])->eProgOrder;
			pImageInfo->nXTilesCount = pCodingParams->nXTilesCount;
			pImageInfo->nYTilesCount = pCodingParams->nYTilesCount;
			pImageInfo->nXTsiz       = pCodingParams->nXTsiz;
			pImageInfo->nYTsiz       = pCodingParams->nYTsiz;
			pImageInfo->nXTOsiz      = pCodingParams->nXTOsiz;
			pImageInfo->nYTOsiz      = pCodingParams->nYTOsiz;
			pImageInfo->nCompCount   = pImage->nCsiz;
			pImageInfo->nLayersCount = (&pCodingParams->pTCP[0])->nLayersCount;
			pImageInfo->nDecompCount = (&pCodingParams->pTCP[0])->pTCCP->nResolutionsCount - 1;
			pImageInfo->dDistoMax    = 0;
		}
		//-------------

		J2k_WriteSOC(pJ2k);
		J2k_WriteSIZ(pJ2k);
		J2k_WriteCOD(pJ2k);
		J2k_WriteQCD(pJ2k);

		for (int nComponentIndex = 0; nComponentIndex < pImage->nCsiz; nComponentIndex++)
		{
			TileCodingParams *pTCP = &pCodingParams->pTCP[0];
			if (pTCP->pTCCP[nComponentIndex].nROIShift)
				J2k_WriteRGN(pJ2k, nComponentIndex, 0);
		}
		if (pCodingParams->sComment != NULL)
		{
			J2k_WriteCOM(pJ2k);
		}

		// Индексация
		if (pImageInfo && pImageInfo->nIndexOn)
		{
			pImageInfo->nMainHeadEnd =  pStream->Tell() - 1;
		}
		//-------------

		// Создаем струкртуру для кодирования тайлов
		TCD *pTCD = TCD_Create(pJ2k->pCodecInfo);

		// Кодируем каждый тайл
		for (int nTileIndex = 0; nTileIndex < pCodingParams->nXTilesCount * pCodingParams->nYTilesCount; nTileIndex++)
		{
			Event_Message(EVT_INFO, "tile number %d / %d\n", nTileIndex + 1, pCodingParams->nXTilesCount * pCodingParams->nYTilesCount);

			pJ2k->nCurTileIndex = nTileIndex;

			// Инициализируем перед кодированием
			if (0 == nTileIndex)
			{
				TCD_MallocEncode(pTCD, pImage, pCodingParams, pJ2k->nCurTileIndex);
			}
			else
			{
				TCD_InitEncode(pTCD, pImage, pCodingParams, pJ2k->nCurTileIndex);
			}
			// Проверяем ошибку
			if (JP2_ERROR_NO_ERROR != pTCD->pCodecInfo->nErrorCode)
			{
				TCD_Destroy(pTCD);
				return false;
			}

			// Индексация
			if (pImageInfo && pImageInfo->nIndexOn)
			{
				pImageInfo->pTile[pJ2k->nCurTileIndex].nTileCount = pJ2k->nCurTileIndex;
				pImageInfo->pTile[pJ2k->nCurTileIndex].nStartPos  =  pStream->Tell() + pJ2k->nPosCorrection;
			}
			//-------------

			J2k_WriteSOT(pJ2k);

			for (int nComponentIndex = 1; nComponentIndex < pImage->nCsiz; nComponentIndex++)
			{
				J2k_WriteCOC(pJ2k, nComponentIndex);
				J2k_WriteQCC(pJ2k, nComponentIndex);
			}
			if (pCodingParams->pTCP[nTileIndex].nPOCsCount)
			{
				J2k_WritePOC(pJ2k);
			}

			J2k_WriteSOD(pJ2k, pTCD);

			// Индексация
			if (pImageInfo && pImageInfo->nIndexOn)
			{
				pImageInfo->pTile[pJ2k->nCurTileIndex].nEndPos =  pStream->Tell() + pJ2k->nPosCorrection - 1;
			}
			//-------------

		}

		// Удаляем структуру кодирующую тайлы
		TCD_FreeEncode(pTCD);
		TCD_Destroy(pTCD);

		J2k_WriteEOC(pJ2k);

		// Создаем файл с индексацией
		if (pImageInfo && pImageInfo->nIndexOn)
		{
			if (!J2k_CreateIndex(pJ2k, pStream, pImageInfo, sIndex))
			{
				Event_Message(EVT_ERROR, "failed to create index file %s\n", sIndex);
				return false;
			}
		}


		return true;
	}
}
