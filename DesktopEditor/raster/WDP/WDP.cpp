#include "WDP.h"

#include "../../common/File.h"

namespace WDP {

	bool CWDPFile::Open(CBgraFrame* pFrame, const std::wstring& strFileName, bool isRGBA)
    {
		ERR err = WMP_errSuccess;

		PKFactory* pFactory = NULL;
		PKCodecFactory* pCodecFactory = NULL;
		PKImageDecode* pDecoder = NULL;

		char* pExt = NULL;
		U32 cFrame = 0;
		U32 i = 0;
		int uAlphaMode = 255;
		PKPixelInfo PI;

		if (PKCreateFactory(&pFactory, PK_SDK_VERSION) != WMP_errSuccess)
			return false;

		if (PKCreateCodecFactory(&pCodecFactory, WMP_SDK_VERSION) != WMP_errSuccess)
			return false;

		NSFile::CUtf8Converter CConverter;
		std::string convertFileName = CConverter.GetUtf8StringFromUnicode(strFileName);

		if (pCodecFactory->CreateDecoderFromFile(convertFileName.c_str(), &pDecoder) != WMP_errSuccess)
			return false;

        if(pDecoder->WMP.wmiI.cfColorFormat == CMYK) { // CMYK = > RGB
		    pDecoder->WMP.wmiI.cfColorFormat = CF_RGB;
		    pDecoder->guidPixFormat = GUID_PKPixelFormat24bppRGB;
		    pDecoder->WMP.wmiI.bRGB = 1; //RGB
	    }

        PI.pGUIDPixFmt = &GUID_PKPixelFormat24bppRGB;
	    PixelFormatLookup(&PI, LOOKUP_FORWARD);

        if(!!(PI.grBit & PK_pixfmtHasAlpha))
			uAlphaMode = 2;//default is image & alpha for formats with alpha
		else
			uAlphaMode = 0;//otherwise, 0

        pDecoder->WMP.wmiSCP.bfBitstreamFormat = SPATIAL;
		pDecoder->WMP.wmiSCP.uAlphaMode = uAlphaMode;
	    pDecoder->WMP.wmiSCP.sbSubband = SB_ALL;
	    pDecoder->WMP.bIgnoreOverlap = false;

	    pDecoder->WMP.wmiI.cfColorFormat = PI.cfColorFormat;

	    pDecoder->WMP.wmiI.bdBitDepth = PI.bdBitDepth;
	    pDecoder->WMP.wmiI.cBitsPerUnit = PI.cbitUnit;

        //==== Validate thumbnail decode parameters =====
	    pDecoder->WMP.wmiI.cThumbnailWidth = pDecoder->WMP.wmiI.cWidth;
	    pDecoder->WMP.wmiI.cThumbnailHeight = pDecoder->WMP.wmiI.cHeight;
	    pDecoder->WMP.wmiI.bSkipFlexbits = false;
        
        pDecoder->WMP.wmiI.cROILeftX = 0;
	    pDecoder->WMP.wmiI.cROITopY = 0;
	    pDecoder->WMP.wmiI.cROIWidth = pDecoder->WMP.wmiI.cThumbnailWidth;
	    pDecoder->WMP.wmiI.cROIHeight = pDecoder->WMP.wmiI.cThumbnailHeight;

		pDecoder->WMP.wmiI.oOrientation = O_NONE;

        pDecoder->WMP.wmiI.cPostProcStrength = 0;

	    pDecoder->WMP.wmiSCP.bVerbose = false;

        U8* pb = NULL;

	    PKRect rect = {0, 0, 0, 0};
    
        rect.Width = (I32)(pDecoder->WMP.wmiI.cROIWidth);
	    rect.Height = (I32)(pDecoder->WMP.wmiI.cROIHeight);
    
        PKPixelInfo pPIFrom;
	    PKPixelInfo pPITo;
    
        pPIFrom.pGUIDPixFmt = &GUID_PKPixelFormat32bppBGRA;
	    PixelFormatLookup(&pPIFrom, LOOKUP_FORWARD);
    
        pPITo.pGUIDPixFmt = &GUID_PKPixelFormat24bppBGR;
	    PixelFormatLookup(&pPITo, LOOKUP_FORWARD);
    
        U32 cbStrideFrom = (BD_1 == pPIFrom.bdBitDepth ? ((pPIFrom.cbitUnit * rect.Width + 7) >> 3) : (((pPIFrom.cbitUnit + 7) >> 3) * rect.Width));
	    if (&GUID_PKPixelFormat12bppYUV420 == pPIFrom.pGUIDPixFmt
		    || &GUID_PKPixelFormat16bppYUV422 == pPIFrom.pGUIDPixFmt)
		    cbStrideFrom >>= 1;

        U32 cbStrideTo = (BD_1 == pPITo.bdBitDepth ? ((pPITo.cbitUnit * pDecoder->uWidth + 7) >> 3) : (((pPITo.cbitUnit + 7) >> 3) * pDecoder->uWidth));
	    if (&GUID_PKPixelFormat12bppYUV420 == pPITo.pGUIDPixFmt
		    || &GUID_PKPixelFormat16bppYUV422 == pPITo.pGUIDPixFmt)
		    cbStrideTo >>= 1;

        U32 cbStride = max(cbStrideFrom, cbStrideTo);

		if (PKAllocAligned((void **) &pb, cbStride * rect.Height, 128) != WMP_errSuccess)
			return false;
		if (pDecoder->Copy(pDecoder, &rect, pb, cbStride) != WMP_errSuccess)
			return false;

        if (pb == NULL)
            return false;

        int nWidth = rect.Width;
        int nHeight = rect.Height;
        int BufferSize = 4 * nWidth * nHeight;

        if (BufferSize < 1)
            return false;

        pFrame->put_Height(nHeight);
        pFrame->put_Width(nWidth);
        pFrame->put_Stride(4 * nWidth);

        BYTE* pData = new BYTE[BufferSize];

        if (!pData)
            return false;

        pFrame->put_Data(pData);

        unsigned char* pBufferPtr = (unsigned char*)pData;

        unsigned int indR = isRGBA ? 2 : 0;
        unsigned int indG = 1;
        unsigned int indB = isRGBA ? 0 : 2;

        for (size_t i = 0; i < nHeight; i++)
        {
            unsigned char* q = pb + 4 * (i * nWidth);
            for (size_t j = 0; j < nWidth; j++)
            {
                pBufferPtr[indR] = * q;
                pBufferPtr[indG] = * (q + 1);
                pBufferPtr[indB] = * (q + 2);
                pBufferPtr[3] = *(q + 3);
                q += 4;
                pBufferPtr += 4;
            }
        }

        return true;
    }
}
