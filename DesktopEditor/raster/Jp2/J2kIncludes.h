#pragma once

#include "Types.h"
#include "ArithmeticCoder.h"
#include "DWT.h"
#include "JpgEvent.h"
#include "Image.h"
#include "J2k.h"
#include "Jp2.h"
#include "Jpt.h"
#include "Mj2.h"
#include "PacketIterator.h"
#include "Raw.h"
#include "Reader.h"
#include "TagTree.h"
#include "Tile.h"
#include "Tier1.h"
#include "Tier2.h"
#include "Utils.h"

#include <string>
#include "../../common/File.h"

namespace Jpeg2000
{
	//-------------------------------------------------------------------------------------------------------------------------------
	// Jpeg2000 -> Image
	//-------------------------------------------------------------------------------------------------------------------------------
    Image* Jp2ToImage(CReader *pStream, DecoderParams* pDecoderParams)
    {
        DInfo *pDInfo = (DInfo*)Malloc(sizeof(DInfo));
        if (!pDInfo)
            return NULL;

        pDInfo->bIsDecompressor = true;
        pDInfo->pJp2 = (void*)Jp2_CreateDecompress((PCommon)pDInfo);
        if (!pDInfo->pJp2)
        {
            Free(pDInfo);
            return NULL;
        }

        pDInfo->pJ2k = NULL;
        pDInfo->eCodecFormat = codecJP2;

        Jp2_SetupDecoder((Jp2Stream*)pDInfo->pJp2, pDecoderParams);
        if (JP2_ERROR_NO_ERROR != pDInfo->nErrorCode)
        {
            Jp2_DestroyDecompress((Jp2Stream*)pDInfo->pJp2);
            Free(pDInfo);
            return NULL;
        }

        Image *pImage = Jp2_Decode((Jp2Stream*)pDInfo->pJp2, pStream); // в случае ошибки pImage = NULL
        Jp2_DestroyDecompress((Jp2Stream*)pDInfo->pJp2);

        Free(pDInfo);
        return pImage;
    }
    Image* J2kToImage(CReader *pStream, DecoderParams* pDecoderParams)
    {
        DInfo *pDInfo = (DInfo*)Malloc(sizeof(DInfo));
        if (!pDInfo)
            return NULL;

        pDInfo->bIsDecompressor = true;
        pDInfo->pJ2k = (void*)J2k_CreateDecompress((PCommon)pDInfo);
        if (!pDInfo->pJ2k)
        {
            Free(pDInfo);
            return NULL;
        }

        pDInfo->pJp2 = NULL;
        pDInfo->eCodecFormat = codecJ2K;

        J2k_SetupDecoder((J2kCodestream*)pDInfo->pJ2k, pDecoderParams);
        if (JP2_ERROR_NO_ERROR != pDInfo->nErrorCode)
        {
            J2k_DestroyDecompress((J2kCodestream*)pDInfo->pJ2k);
            Free(pDInfo);
            return NULL;
        }

        Image *pImage = J2k_Decode((J2kCodestream*)pDInfo->pJ2k, pStream); // в случае ошибки pImage = NULL
        J2k_DestroyDecompress((J2kCodestream*)pDInfo->pJ2k);

        Free(pDInfo);

        return pImage;
    }
    Image* JptToImage(CReader *pStream, DecoderParams* pDecoderParams)
    {
        DInfo *pDInfo = (DInfo*)Malloc(sizeof(DInfo));
        if (!pDInfo)
            return NULL;

        pDInfo->bIsDecompressor = true;
        pDInfo->pJ2k = (void*)J2k_CreateDecompress((PCommon)pDInfo);
        if (!pDInfo->pJ2k)
        {
            Free(pDInfo);
            return NULL;
        }

        pDInfo->pJp2 = NULL;
        pDInfo->eCodecFormat = codecJPT;

        J2k_SetupDecoder((J2kCodestream*)pDInfo->pJ2k, pDecoderParams);
        if (JP2_ERROR_NO_ERROR != pDInfo->nErrorCode)
        {
            J2k_DestroyDecompress((J2kCodestream*)pDInfo->pJ2k);
            Free(pDInfo);
            return NULL;
        }

        Image *pImage = J2k_DecodeJptStream((J2kCodestream*)pDInfo->pJ2k, pStream); // в случае ошибки pImage = NULL
        J2k_DestroyDecompress((J2kCodestream*)pDInfo->pJ2k);

        Free(pDInfo);
        return pImage;
    }
    Image* Mj2ToImage(CReader *pStream, DecoderParams* pDecoderParams)
    {
        DInfo *pDInfo = (DInfo*)Malloc(sizeof(DInfo));
        if (!pDInfo)
            return NULL;

        pDInfo->bIsDecompressor = true;
        pDInfo->pMj2 = (void*)Mj2_CreateDecompress((PCommon)pDInfo);
        if (!pDInfo->pMj2)
        {
            Free(pDInfo);
            return NULL;
        }

        pDInfo->pJ2k         = ((Mj2_Movie*)pDInfo->pMj2)->pJ2k;
        pDInfo->eCodecFormat = codecMj2;

        Mj2_SetupDecoder((Mj2_Movie*)pDInfo->pMj2, pDecoderParams);
        if (JP2_ERROR_NO_ERROR != pDInfo->nErrorCode)
        {
            Mj2_DestroyDecompress((Mj2_Movie*)pDInfo->pMj2);
            Free(pDInfo);
            return NULL;
        }

        Image *pImage = Mj2_Decode((Mj2_Movie*)pDInfo->pMj2, pStream); // в случае ошибки pImage = NULL
        Mj2_DestroyDecompress((Mj2_Movie*)pDInfo->pMj2);

        Free(pDInfo);
        return pImage;
    }

    // From files
	Image* Jp2ToImage(const std::wstring& wsFileName, DecoderParams* pDecoderParams)
	{
        CReader *pStream = (CReader *)new CReaderFile(wsFileName, 1);
        if (!pStream)
            return NULL;
        Image* pImage = Jp2ToImage(pStream, pDecoderParams);
        delete pStream;
        return pImage;
	}
	Image* J2kToImage(const std::wstring& wsFileName, DecoderParams* pDecoderParams)
	{
        CReader *pStream = (CReader *)new CReaderFile(wsFileName, 1);
        if (!pStream)
            return NULL;
        Image* pImage = J2kToImage(pStream, pDecoderParams);
        delete pStream;
        return pImage;
	}
	Image* JptToImage(const std::wstring& wsFileName, DecoderParams* pDecoderParams)
	{
        CReader *pStream = (CReader *)new CReaderFile(wsFileName, 1);
        if (!pStream)
            return NULL;
        Image* pImage = JptToImage(pStream, pDecoderParams);
        delete pStream;
        return pImage;
	}
	Image* Mj2ToImage(const std::wstring& wsFileName, DecoderParams* pDecoderParams)
	{
        CReader *pStream = (CReader *)new CReaderFile(wsFileName, 1);
        if (!pStream)
            return NULL;
        Image* pImage = Mj2ToImage(pStream, pDecoderParams);
        delete pStream;
        return pImage;
	}

    // From buffer
	Image* Jp2ToImage(BYTE* pBuffer, int nSize, DecoderParams* pDecoderParams)
	{
        CReader *pStream = (CReader *)new CReaderStream(pBuffer, nSize);
        if (!pStream)
            return NULL;
        Image* pImage = Jp2ToImage(pStream, pDecoderParams);
        delete pStream;
        return pImage;
	}
	Image* J2kToImage(BYTE* pBuffer, int nSize, DecoderParams* pDecoderParams)
	{
        CReader *pStream = (CReader *)new CReaderStream(pBuffer, nSize);
        if (!pStream)
            return NULL;
        Image* pImage = J2kToImage(pStream, pDecoderParams);
        delete pStream;
        return pImage;
	}
	Image* JptToImage(BYTE* pBuffer, int nSize, DecoderParams* pDecoderParams)
	{
        CReader *pStream = (CReader *)new CReaderStream(pBuffer, nSize);
        if (!pStream)
            return NULL;
        Image* pImage = JptToImage(pStream, pDecoderParams);
        delete pStream;
        return pImage;
	}
	Image* Mj2ToImage(BYTE* pBuffer, int nSize, DecoderParams* pDecoderParams)
	{
        CReader *pStream = (CReader *)new CReaderStream(pBuffer, nSize);
        if (!pStream)
            return NULL;
        Image* pImage = Mj2ToImage(pStream, pDecoderParams);
        delete pStream;
        return pImage;
	}
	//-------------------------------------------------------------------------------------------------------------------------------
	// Image -> Jpeg2000
	//-------------------------------------------------------------------------------------------------------------------------------
	bool ImageToJ2k(Image *pImage, const std::wstring& wsFilePath, EncoderParams* pEncoderParams)
	{
		int nCodeStreamLen = 0;
		CReader *pStream = NULL;
		CInfo *pCInfo = (CInfo*)Malloc(sizeof(CInfo));
		if (!pCInfo)
			return false;

		pCInfo->pJ2k = (void*)J2k_CreateCompress((PCommon)pCInfo);
		if (!pCInfo->pJ2k)
		{
			Free(pCInfo);
			return false;
		}
		pCInfo->pJp2 = NULL;
		pCInfo->eCodecFormat = codecJ2K;
		pCInfo->bIsDecompressor = false;

		J2k_SetupEncoder((J2kCodestream*)pCInfo->pJ2k, pEncoderParams, pImage);
		if (JP2_ERROR_NO_ERROR != pCInfo->nErrorCode)
		{
			J2k_DestroyCompress((J2kCodestream*)pCInfo->pJ2k);
			Free(pCInfo);
			return false;
		}

		pStream = (CReader *)new CReaderFile(wsFilePath, 0);
		if (!pStream)
		{
			J2k_DestroyCompress((J2kCodestream*)pCInfo->pJ2k);
			Free(pCInfo);
			return false;
		}

		bool bSuccess = J2k_Encode((J2kCodestream*)pCInfo->pJ2k, pStream, pImage, NULL);
		if (!bSuccess)
		{
			delete (pStream);
			J2k_DestroyCompress((J2kCodestream*)pCInfo->pJ2k);
			Free(pCInfo);
			return false;
		}
		nCodeStreamLen =  pStream->Tell();

		//if (!oFile.CreateFileW(wsFilePath))
		//{
		//	delete (pStream);
		//	J2k_DestroyCompress((J2kCodestream*)pCInfo->pJ2k);
		//	Free(pCInfo);
		//	return false;
		//}
		//FILE* pFile = oFile.GetFileNative();
		//fwrite(pStream->GetOwner(), 1, nCodeStreamLen, pFile);
		//fclose(pFile);

		delete (pStream);
		J2k_DestroyCompress((J2kCodestream*)pCInfo->pJ2k);
		Free(pCInfo);

		return true;
	}
	bool ImageToJ2p(Image *pImage, const std::wstring& wsFilePath, EncoderParams* pEncoderParams)
	{
		int nCodeStreamLen = 0;
		CReader *pStream = NULL;
		CInfo *pCInfo = (CInfo*)Malloc(sizeof(CInfo));
		if (!pCInfo)
			return false;

		pCInfo->pJp2 = (void*)Jp2_CreateCompress((PCommon)pCInfo);
		if (!pCInfo->pJp2)
		{
			Free(pCInfo);
			return false;
		}
		pCInfo->pJ2k = NULL;
		pCInfo->eCodecFormat = codecJP2;
		pCInfo->bIsDecompressor = false;

		Jp2_SetupEncoder((Jp2Stream*)pCInfo->pJp2, pEncoderParams, pImage);
		if (JP2_ERROR_NO_ERROR != pCInfo->nErrorCode)
		{
			Jp2_DestroyCompress((Jp2Stream*)pCInfo->pJp2);
			Free(pCInfo);
			return false;
		}

		pStream =(CReader *) new CReaderStream(NULL, 0);
		if (!pStream)
		{
			Jp2_DestroyCompress((Jp2Stream*)pCInfo->pJp2);
			Free(pCInfo);
			return false;
		}

		bool bSuccess = Jp2_Encode((Jp2Stream*)pCInfo->pJp2, pStream, pImage, NULL);
		if (!bSuccess)
		{
			delete (pStream);
			Jp2_DestroyCompress((Jp2Stream*)pCInfo->pJp2);
			Free(pCInfo);
			return false;
		}
		nCodeStreamLen =  pStream->Tell();

		NSFile::CFileBinary oFile;
		if (!oFile.CreateFileW(wsFilePath))
		{
			delete (pStream);
			Jp2_DestroyCompress((Jp2Stream*)pCInfo->pJp2);
			Free(pCInfo);
			return false;
		}
		FILE* pFile = oFile.GetFileNative();
		fwrite(pStream->GetOwner(), 1, nCodeStreamLen, pFile);

		delete (pStream);
		Jp2_DestroyCompress((Jp2Stream*)pCInfo->pJp2);
		Free(pCInfo);

		return true;
	}
}
