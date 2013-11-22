// AVSVideoFramePainter.h 

#pragma once
#include "resource.h"       // main symbols
#include "AVSUncompressedVideoFrame.h"
#include "AVSVideoFrameTransform.h" 
#include <math.h>


// IAVSVideoFrameDrawing
[ object, uuid("AE1D7D3F-ED9D-4159-922A-024F1F45B274"), dual, pointer_default(unique)]
__interface IAVSVideoFramePainter : IDispatch
{
	//в pMask одна байтовая плоскость с альфо каналом
	[id(209000 + 10)] HRESULT DrawMasked([in] IAVSUncompressedVideoFrame* pFrame, [in] long Left, [in] long Top, [in] IAVSUncompressedVideoFrame* pPicture, [in] IAVSUncompressedVideoFrame* pMask);
	[id(209000 + 20)] HRESULT DrawMaskedUnknown([in] IUnknown* pFrame, [in] long Left, [in] long Top, [in] IUnknown* pPicture, [in] IUnknown* pMask);
	

	//Для CSP_BGRA и CSP_BGR	ColorKey передается как 0x00RRGGBB
	//Для CSP_I420 и CSP_YV12	ColorKey передается как 0x00YYUUVV
	//Для CSP_YUY2				ColorKey передается как 0x00YYUUVV
	[id(209000 + 30)] HRESULT DrawColorKey([in] IAVSUncompressedVideoFrame* pFrame, [in] long Left, [in] long Top, [in] IAVSUncompressedVideoFrame* pPicture, [in] long ColorKey);
	[id(209000 + 40)] HRESULT DrawColorKeyUnknown([in] IUnknown* pFrame, [in] long Left, [in] long Top, [in] IUnknown* pPicture, [in] long ColorKey);

	[id(209000 + 100)] HRESULT ConvetColorKey([in] long SrcColorSpace, [in] long DstColorSpace, [in] long SrcColorKey, [out, retval] long *pDstColorKey);
};



// CAVSVideoFrameColorTransform
[ coclass, threading(apartment), vi_progid("AVSMediaCore3.IAVSVideoFramePainter"), progid("AVSMediaCore3.IAVSVideoFramePainter.1"), version(1.0), uuid("DFACD890-DDCB-4432-921E-E46BDA97DE02")]
class ATL_NO_VTABLE CAVSVideoFramePainter : 
	public IAVSVideoFramePainter
{
public:
	CAVSVideoFramePainter()
	{
	}
public:
	STDMETHOD(DrawMasked)(IAVSUncompressedVideoFrame* pFrame, long Left, long Top, IAVSUncompressedVideoFrame* pPicture, IAVSUncompressedVideoFrame* pMask)
	{
		SFrameSetting oFrameSetting;
		if (!GetFrameProperties(pFrame, oFrameSetting, TRUE))
			return S_FALSE;
		SFrameSetting oPictureSetting;
		if (!GetFrameProperties(pPicture, oPictureSetting, TRUE))
			return S_FALSE;

		if (0!=(CSP_VFLIP & oFrameSetting.m_lColorSpace))
			Top = oFrameSetting.m_lHeight - (Top + oPictureSetting.m_lHeight - 1);

		if ((0==oPictureSetting.m_lWidth)||(0==oPictureSetting.m_lHeight)||
			(0==oFrameSetting.m_lWidth)||(0==oFrameSetting.m_lHeight))
			return S_FALSE;
		if ((oFrameSetting.m_lWidth<=Left)||(oFrameSetting.m_lHeight<=Top))
			return S_OK;

		if ((Left<0) && (oPictureSetting.m_lWidth + Left<=0))
			return S_OK;
		if ((Top<0) && (oPictureSetting.m_lHeight + Top<=0))
			return S_OK;

		if (NULL!=pMask)
		{
			SFrameSetting oMaskSetting;
			if (!GetFrameProperties(pMask, oMaskSetting))
				return S_FALSE;
			if ((oPictureSetting.m_lWidth != oMaskSetting.m_lWidth)||
				(oPictureSetting.m_lHeight != oMaskSetting.m_lHeight))
				return S_FALSE;
			if ((CSP_YV12 != (CSP_COLOR_MASK & oMaskSetting.m_lColorSpace))&&
				(CSP_I420 != (CSP_COLOR_MASK & oMaskSetting.m_lColorSpace)))
				return S_FALSE;
		}

		pPicture->AddRef();
		if (oFrameSetting.m_lColorSpace!=oPictureSetting.m_lColorSpace)
		{
			IAVSVideoFrameTransform *pTransform = NULL;
			CoCreateInstance(__uuidof(CAVSVideoFrameTransform), NULL, CLSCTX_ALL, __uuidof(IAVSVideoFrameTransform), (void**)&pTransform);
			if (NULL==pTransform)
				return S_FALSE;
			MediaFormat::IAVSVideoFormat *pFormat = NULL;
			CoCreateInstance(MediaFormat::CLSID_CAVSVideoFormat, NULL, CLSCTX_ALL, MediaFormat::IID_IAVSVideoFormat, (void**)&pFormat);
			if (NULL==pFormat)
			{
				pTransform->Release();
				return S_FALSE;
			}
			pTransform->put_DeinterlaceEnabled(FALSE);
			pTransform->put_CropScale(0);

			pFormat->put_ColorSpace(oFrameSetting.m_lColorSpace);
			pFormat->put_Width(oPictureSetting.m_lWidth);
			pFormat->put_Height(oPictureSetting.m_lHeight);
			long lTemp = 0;
			pPicture->get_AspectRatioX(&lTemp);
			pFormat->put_AspectRatioX(lTemp);
			pPicture->get_AspectRatioY(&lTemp);
			pFormat->put_AspectRatioY(lTemp);
			pTransform->SetVideoFormat(pFormat);
			RELEASEINTERFACE(pFormat);
			IUnknown* punkPicture = NULL;
			pTransform->TransformFrame(pPicture, &punkPicture);
			RELEASEINTERFACE(pTransform);
			RELEASEINTERFACE(pPicture);
			if (NULL==punkPicture)
				return S_FALSE;
			punkPicture->QueryInterface(__uuidof(IAVSUncompressedVideoFrame), (void**)&pPicture);
			punkPicture->Release();
		}
				
		if (NULL!=pMask)
			DrawInner(pFrame, Left, Top, pPicture, pMask);
		else
			DrawInner(pFrame, Left, Top, pPicture);

		RELEASEINTERFACE(pPicture);
		return S_OK;
	}
	STDMETHOD(DrawMaskedUnknown)(IUnknown* punkFrame, long Left, long Top, IUnknown* punkPicture, IUnknown* punkMask)
	{
		IAVSUncompressedVideoFrame* pFrame = NULL;
		IAVSUncompressedVideoFrame* pPicture = NULL;
		IAVSUncompressedVideoFrame* pMask = NULL;
		if ((NULL==punkFrame)||(NULL==punkPicture))
			return S_FALSE;
		punkFrame->QueryInterface(__uuidof(IAVSUncompressedVideoFrame), (void**)&pFrame);
		punkPicture->QueryInterface(__uuidof(IAVSUncompressedVideoFrame), (void**)&pPicture);
		if (NULL!=punkMask)
			punkMask->QueryInterface(__uuidof(IAVSUncompressedVideoFrame), (void**)&pMask);
		HRESULT hr = DrawMasked(pFrame, Left, Top, pPicture, pMask);
		RELEASEINTERFACE(pFrame);
		RELEASEINTERFACE(pPicture);
		RELEASEINTERFACE(pMask);
		return hr;
	}

	STDMETHOD(DrawColorKey)(IAVSUncompressedVideoFrame* pFrame, long Left, long Top, IAVSUncompressedVideoFrame* pPicture, long ColorKey)
	{
		SFrameSetting oFrameSetting;
		if (!GetFrameProperties(pFrame, oFrameSetting, TRUE))
			return S_FALSE;
		SFrameSetting oPictureSetting;
		if (!GetFrameProperties(pPicture, oPictureSetting, TRUE))
			return S_FALSE;

		if (0!=(CSP_VFLIP & oFrameSetting.m_lColorSpace))
			Top = oFrameSetting.m_lHeight - (Top + oPictureSetting.m_lHeight - 1);
		if ((Left<0)||(Top<0))
			return FALSE;
		if ((0==oPictureSetting.m_lWidth)||(0==oPictureSetting.m_lHeight)||
			(0==oFrameSetting.m_lWidth)||(0==oFrameSetting.m_lHeight))
			return S_FALSE;
		if ((oFrameSetting.m_lWidth<=Left)||(oFrameSetting.m_lHeight<=Top))
			return S_OK;

		pPicture->AddRef();
		if (oFrameSetting.m_lColorSpace!=oPictureSetting.m_lColorSpace)
		{
			IAVSVideoFrameTransform *pTransform = NULL;
			CoCreateInstance(__uuidof(CAVSVideoFrameTransform), NULL, CLSCTX_ALL, __uuidof(IAVSVideoFrameTransform), (void**)&pTransform);
			if (NULL==pTransform)
				return S_FALSE;
			MediaFormat::IAVSVideoFormat *pFormat = NULL;
			CoCreateInstance(MediaFormat::CLSID_CAVSVideoFormat, NULL, CLSCTX_ALL, MediaFormat::IID_IAVSVideoFormat, (void**)&pFormat);
			if (NULL==pFormat)
			{
				pTransform->Release();
				return S_FALSE;
			}
			pTransform->put_DeinterlaceEnabled(FALSE);
			pTransform->put_CropScale(0);

			pFormat->put_ColorSpace(oFrameSetting.m_lColorSpace);
			pFormat->put_Width(oPictureSetting.m_lWidth);
			pFormat->put_Height(oPictureSetting.m_lHeight);
			long lTemp = 0;
			pPicture->get_AspectRatioX(&lTemp);
			pFormat->put_AspectRatioX(lTemp);
			pPicture->get_AspectRatioY(&lTemp);
			pFormat->put_AspectRatioY(lTemp);
			pTransform->SetVideoFormat(pFormat);
			RELEASEINTERFACE(pFormat);
			IUnknown* punkPicture = NULL;
			pTransform->TransformFrame(pPicture, &punkPicture);
			RELEASEINTERFACE(pTransform);
			RELEASEINTERFACE(pPicture);
			if (NULL==punkPicture)
				return S_FALSE;
			punkPicture->QueryInterface(__uuidof(IAVSUncompressedVideoFrame), (void**)&pPicture);
			punkPicture->Release();
		}
		DrawSimpleInner(pFrame, Left, Top, pPicture, ConvertColorKey(ColorKey, oPictureSetting.m_lColorSpace, oFrameSetting.m_lColorSpace));
		RELEASEINTERFACE(pPicture);
		return S_OK;
	}

	STDMETHOD(DrawColorKeyUnknown)(IUnknown* punkFrame, long Left, long Top, IUnknown* punkPicture, long ColorKey)
	{
		IAVSUncompressedVideoFrame* pFrame = NULL;
		IAVSUncompressedVideoFrame* pPicture = NULL;
		if ((NULL==punkFrame)||(NULL==punkPicture))
			return S_FALSE;
		punkFrame->QueryInterface(__uuidof(IAVSUncompressedVideoFrame), (void**)&pFrame);
		punkPicture->QueryInterface(__uuidof(IAVSUncompressedVideoFrame), (void**)&pPicture);
		HRESULT hr = DrawColorKey(pFrame, Left, Top, pPicture, ColorKey);
		RELEASEINTERFACE(pFrame);
		RELEASEINTERFACE(pPicture);
		return hr;
	}
	STDMETHOD(ConvetColorKey)(long SrcColorSpace, long DstColorSpace, long SrcColorKey, long *pDstColorKey)
	{
		*pDstColorKey = ConvertColorKey(SrcColorKey, SrcColorSpace, DstColorSpace);
		return S_OK;
	}
protected:	
	struct SFrameSetting
	{
		SFrameSetting()
			: m_lColorSpace(0)
			, m_lHeight(0)
			, m_lWidth(0)
		{
		}
		long m_lColorSpace; 
		long m_lHeight;
		long m_lWidth;		
	};
	static inline BOOL GetFrameProperties(IAVSUncompressedVideoFrame* pFrame, SFrameSetting &oFrameSetting, BOOL bCheckBuffer = FALSE)
	{
		if (NULL==pFrame)
			return FALSE;
		pFrame->get_ColorSpace(&oFrameSetting.m_lColorSpace);
		pFrame->get_Width(&oFrameSetting.m_lWidth);
		pFrame->get_Height(&oFrameSetting.m_lHeight);
		if (!bCheckBuffer)
			return TRUE;
		switch (oFrameSetting.m_lColorSpace & CSP_COLOR_MASK)
		{
		case CSP_I420:
		case CSP_YV12:
			{
				LPBYTE pTemp = NULL;
				pFrame->get_Plane(0, &pTemp);
				if (NULL==pTemp)
					return FALSE;
				pTemp = NULL;
				pFrame->get_Plane(1, &pTemp);
				if (NULL==pTemp)
					return FALSE;
				pTemp = NULL;
				pFrame->get_Plane(2, &pTemp);
				if (NULL==pTemp)
					return FALSE;
			}
			break;
		case CSP_YUY2:
		case CSP_BGRA:
		case CSP_BGR:
			{
				LPBYTE pTemp = NULL;
				pFrame->get_Plane(0, &pTemp);
				if (NULL==pTemp)
					return FALSE;
			}
			break;
		default:
			return FALSE;
		}
		return TRUE;
	}
	static void DrawInner(IAVSUncompressedVideoFrame* pFrame, long lLeft, long lTop, IAVSUncompressedVideoFrame* pPicture, IAVSUncompressedVideoFrame* pMask)
	{
		SFrameSetting oFrameSetting;
		GetFrameProperties(pFrame, oFrameSetting);
		SFrameSetting oPictureSetting;
		GetFrameProperties(pPicture, oPictureSetting);

		long lMaskColorSpace;
		pMask->get_ColorSpace(&lMaskColorSpace);

		switch (oFrameSetting.m_lColorSpace & CSP_COLOR_MASK)
		{
		case CSP_I420:
		case CSP_YV12:
			{
				if (lLeft<0)
					lLeft = -(abs(lLeft)>>1)<<1;
				else
					lLeft = (lLeft>>1)<<1;
				if (lTop<0)
					lTop = -(abs(lTop)>>1)<<1;
				else
					lTop = (lTop>>1)<<1;
				
				long lPictureWidth = (min(oPictureSetting.m_lWidth, oFrameSetting.m_lWidth - lLeft)>>1)<<1;
				if (lLeft<0)
					lPictureWidth += lLeft;
				long lPictureHeight = (min(oPictureSetting.m_lHeight, oFrameSetting.m_lHeight - lTop)>>1)<<1;
				if (lTop<0)
					lPictureHeight += lTop;

				LPBYTE pMaskPlane = NULL;
				pMask->get_Plane(0, &pMaskPlane);
		

				long lMaskStride;
				pMask->get_Stride(0, &lMaskStride);
				{//YPlane
					LPBYTE pFrameY = NULL;
					pFrame->get_Plane(0, &pFrameY);
					long lFrameStrideY;
					pFrame->get_Stride(0, &lFrameStrideY);

					LPBYTE pPictureY = NULL;
					pPicture->get_Plane(0, &pPictureY);
					long lPictureStrideY;
					pPicture->get_Stride(0, &lPictureStrideY);

					LPBYTE pMaskPtr = pMaskPlane;
					long lFramePitch = lFrameStrideY - lPictureWidth;
					long lPicturePitch = lPictureStrideY - lPictureWidth;
					long lMaskPitch = lMaskStride - lPictureWidth;

					if (lLeft<0)
					{
						pMaskPtr -= lLeft;
						pPictureY -= lLeft;
					}
					else
						pFrameY += lLeft;

					if (0!=(oFrameSetting.m_lColorSpace & CSP_VFLIP))
					{
						lFramePitch		-= 2*lFrameStrideY;
						lPicturePitch	-= 2*lPictureStrideY;
						pPictureY += (oPictureSetting.m_lHeight - 1)*lPictureStrideY;
						pFrameY += (oFrameSetting.m_lHeight - 1)*lFrameStrideY;
						if (lTop<0)
							pPictureY += lTop*lPictureStrideY;
						else
							pFrameY -= lTop*lFrameStrideY;
					}
					else
					{
						if (lTop<0)
							pPictureY -= lTop*lPictureStrideY;
						else
							pFrameY += lTop*lFrameStrideY;
					}

					if (0!=(lMaskColorSpace & CSP_VFLIP))
					{
						lMaskPitch -= 2*lMaskStride;
						pMaskPtr += (oPictureSetting.m_lHeight-1)*lMaskStride;
						if (lTop<0)
							pMaskPtr += lTop*lMaskStride;
					}
					else
					{
						if (lTop<0)
							pMaskPtr -= lTop*lMaskStride;
					}

					for (long lRow = 0; lRow<lPictureHeight; lRow++)
					{
						for (long lCol = 0; lCol<lPictureWidth; lCol++, pFrameY++, pPictureY++, pMaskPtr++)
						{
							*pFrameY = ((255-(*pMaskPtr))*(*pFrameY) + (*pMaskPtr)*(*pPictureY))/255;							
						}
						pFrameY		+= lFramePitch;
						pPictureY	+= lPicturePitch;
						pMaskPtr	+= lMaskPitch;
					}
				}
				
				{//UVPlane
					LPBYTE pFrameU = NULL;
					LPBYTE pFrameV = NULL;
					pFrame->get_Plane(1, &pFrameU);
					pFrame->get_Plane(2, &pFrameV);
					long lFrameStrideU;
					long lFrameStrideV;
					pFrame->get_Stride(1, &lFrameStrideU);
					pFrame->get_Stride(2, &lFrameStrideV);

				
					LPBYTE pPictureU = NULL;
					LPBYTE pPictureV = NULL;
					pPicture->get_Plane(1, &pPictureU);
					pPicture->get_Plane(2, &pPictureV);

					long lPictureStrideU;
					long lPictureStrideV;
					pPicture->get_Stride(1, &lPictureStrideU);
					pPicture->get_Stride(2, &lPictureStrideV);

					LPBYTE pMaskPtr = pMaskPlane;
					long lFramePitchU = lFrameStrideU - lPictureWidth/2;
					long lFramePitchV = lFrameStrideV - lPictureWidth/2;
					long lPicturePitchU = lPictureStrideU - lPictureWidth/2;
					long lPicturePitchV = lPictureStrideV - lPictureWidth/2;
					long lMaskPitch = 2*lMaskStride - lPictureWidth;
					if (lLeft<0)
					{
						pMaskPtr -= lLeft;
						pPictureU -= lLeft/2;
						pPictureV -= lLeft/2;
					}
					else
					{
						pFrameU += lLeft/2;
						pFrameV += lLeft/2;
					}
					if (0!=(oFrameSetting.m_lColorSpace & CSP_VFLIP))
					{
						lFramePitchU -= 2*lFrameStrideU;
						lFramePitchV -= 2*lFrameStrideV;
						lPicturePitchU -= 2*lPictureStrideU;
						lPicturePitchV -= 2*lPictureStrideV;

						pFrameU += lFrameStrideU*(oFrameSetting.m_lHeight/2 - 1);
						pFrameV += lFrameStrideV*(oFrameSetting.m_lHeight/2 - 1);

						pPictureU += (oPictureSetting.m_lHeight/2 - 1)*lPictureStrideU;
						pPictureV += (oPictureSetting.m_lHeight/2 - 1)*lPictureStrideV;

						if (lTop<0)
						{
							pPictureU += lTop/2*lPictureStrideU;
							pPictureV += lTop/2*lPictureStrideV;
						}
						else
						{
							pFrameU -= lTop/2*lFrameStrideU;
							pFrameV -= lTop/2*lFrameStrideV;
						}
					}
					else
					{
						if (lTop<0)
						{
							pPictureU -= lTop/2*lPictureStrideU;
							pPictureV -= lTop/2*lPictureStrideV;
						}
						else
						{
							pFrameU += lTop/2*lFrameStrideU;
							pFrameV += lTop/2*lFrameStrideV;
						}
					}
					if (0!=(lMaskColorSpace & CSP_VFLIP))
					{
						lMaskPitch -= 4*lMaskStride;
						pMaskPtr += (oPictureSetting.m_lHeight-1)*lMaskStride;
						if (lTop<0)
							pMaskPtr += lTop*lMaskStride;
					}
					else
					{
						if (lTop<0)
							pMaskPtr -= lTop*lMaskStride;
					}

					//long lPictureTempWidth = lPictureWidth/2;
					//if (lLeft<0)
					//	lPictureTempWidth += lLeft/2;
					//long lPictureTempHeight = lPictureHeight/2;
					//if (lTop<0)
					//	lPictureTempHeight += lTop/2;
					for (long lRow = 0; lRow<lPictureHeight/2; lRow++)
					{
						for (long lCol = 0; lCol<lPictureWidth/2; lCol++, pFrameU++, pFrameV++, pPictureU++, pPictureV++, pMaskPtr+=2)
						{
							*pFrameU = ((255-(*pMaskPtr))*(*pFrameU) + (*pMaskPtr)*(*pPictureU))/255;							
							*pFrameV = ((255-(*pMaskPtr))*(*pFrameV) + (*pMaskPtr)*(*pPictureV))/255;							
						}
						pFrameU		+= lFramePitchU;
						pFrameV		+= lFramePitchV;
						pPictureU	+= lPicturePitchU;
						pPictureV	+= lPicturePitchV;
						pMaskPtr	+= lMaskPitch;
					}
				}
			}
			break;
		case CSP_YUY2:
			{
				if (lLeft<0)
					lLeft = -(abs(lLeft)>>1)<<1;
				else
					lLeft = (lLeft>>1)<<1;
				if (lTop<0)
					lTop = -(abs(lTop)>>1)<<1;
				else
					lTop = (lTop>>1)<<1;

				long lPictureWidth = (min(oPictureSetting.m_lWidth, oFrameSetting.m_lWidth - lLeft)>>1)<<1;
				if (lLeft<0)
					lPictureWidth += lLeft;
				long lPictureHeight = (min(oPictureSetting.m_lHeight, oFrameSetting.m_lHeight - lTop)>>1)<<1;
				if (lTop<0)
					lPictureHeight += lTop;

				LPBYTE pMaskPlane = NULL;
				pMask->get_Plane(0, &pMaskPlane);
				long lMaskStride;
				pMask->get_Stride(0, &lMaskStride);

				LPBYTE pFramePtr = NULL;
				pFrame->get_Plane(0, &pFramePtr);
				long lFrameStride;
				pFrame->get_Stride(0, &lFrameStride);

				LPBYTE pPicturePtr = NULL;
				pPicture->get_Plane(0, &pPicturePtr);
				long lPictureStride;
				pPicture->get_Stride(0, &lPictureStride);

				LPBYTE pMaskPtr = pMaskPlane;
				long lFramePitch = lFrameStride - 2*lPictureWidth;
				long lPicturePitch = lPictureStride - 2*lPictureWidth;
				long lMaskPitch = lMaskStride - lPictureWidth;
				if (lLeft>=0)
					pFramePtr += 2*lLeft;
				else
				{
					pPicturePtr -= 2*lLeft;
					pMaskPtr -= 2*lLeft;
				}

				if (0!=oFrameSetting.m_lColorSpace & CSP_VFLIP)
				{
					lFramePitch		-= 2*lFrameStride;
					lPicturePitch	-= 2*lPictureStride;
					pFramePtr += (oFrameSetting.m_lHeight - 1)*lFrameStride;
					pPicturePtr += (oPictureSetting.m_lHeight - 1)*lPictureStride;
					if (lTop>=0)
						pFramePtr -= lTop * lFrameStride;
					else
						pPicturePtr += lTop * lPictureStride;
				}
				else
				{
					if (lTop>=0)
						pFramePtr += lTop * lFrameStride;
					else
						pPicturePtr -= lTop * lPictureStride;
				}
				if (0!=lMaskColorSpace & CSP_VFLIP)
				{
					lMaskPitch -= 2*lMaskStride;
					pMaskPtr += (oFrameSetting.m_lHeight-1)*lMaskStride;
					if (lTop<0)
						pMaskPtr += lTop*lMaskStride;
				}
				else
				{
					if (lTop<0)
						pMaskPtr -= lTop*lMaskStride;
				}

				for (long lRow = 0; lRow<lPictureHeight; lRow++)
				{
					for (long lCol = 0; lCol<lPictureWidth; lCol++, pMaskPtr++)
					{
						*pFramePtr = ((255-(*pMaskPtr))*(*pFramePtr) + (*pMaskPtr)*(*pPicturePtr))/255;
						pFramePtr++; pPicturePtr++;
						*pFramePtr = ((255-(*pMaskPtr))*(*pFramePtr) + (*pMaskPtr)*(*pPicturePtr))/255;
						pFramePtr++; pPicturePtr++;
					}
					pFramePtr	+= lFramePitch;
					pPicturePtr	+= lPicturePitch;
					pMaskPtr	+= lMaskPitch;
				}
			}
			break;
		case CSP_BGRA:
			{
				long lPictureWidth = min(oPictureSetting.m_lWidth, oFrameSetting.m_lWidth - lLeft);
				if (lLeft<0)
					lPictureWidth += lLeft;
				long lPictureHeight = min(oPictureSetting.m_lHeight, oFrameSetting.m_lHeight - lTop);
				if (lTop<0)
					lPictureHeight += lTop;

				LPBYTE pMaskPlane = NULL;
				pMask->get_Plane(0, &pMaskPlane);
				long lMaskStride;
				pMask->get_Stride(0, &lMaskStride);
			
				LPBYTE pFramePtr = NULL;
				pFrame->get_Plane(0, &pFramePtr);
				long lFrameStride;
				pFrame->get_Stride(0, &lFrameStride);

				LPBYTE pPicturePtr = NULL;
				pPicture->get_Plane(0, &pPicturePtr);
				long lPictureStride;
				pPicture->get_Stride(0, &lPictureStride);

				LPBYTE pMaskPtr = pMaskPlane;
				long lFramePitch = lFrameStride - 4*lPictureWidth;
				long lPicturePitch = lPictureStride - 4*lPictureWidth;
				long lMaskPitch = lMaskStride - lPictureWidth;
				if (lLeft>=0)
					pFramePtr += 4*lLeft;
				else
				{
					pPicturePtr -= 4*lLeft;
					pMaskPtr -= lLeft;
				}

				if (0!=oFrameSetting.m_lColorSpace & CSP_VFLIP)
				{
					lFramePitch		-= 2*lFrameStride;
					lPicturePitch	-= 2*lPictureStride;
					pFramePtr += (oFrameSetting.m_lHeight - 1)*lFrameStride;
					pPicturePtr += (oPictureSetting.m_lHeight - 1)*lPictureStride;
					if (lTop>=0)
						pFramePtr -= lTop*lFrameStride;
					else
						pPicturePtr += lTop*lPictureStride;
				}
				else
				{
					if (lTop>=0)
						pFramePtr += lTop*lFrameStride;
					else
						pPicturePtr -= lTop*lPictureStride;
				}
				if (0!=lMaskColorSpace & CSP_VFLIP)
				{
					lMaskPitch -= 2*lMaskStride;
					pMaskPtr += (oFrameSetting.m_lHeight-1)*lMaskStride;
					if (lTop<0)
						pMaskPtr += lTop*lMaskStride;
				}
				else
				{
					if (lTop<0)
						pMaskPtr -= lTop*lMaskStride;
				}
				
				for (long lRow = 0; lRow<lPictureHeight; lRow++)
				{
					for (long lCol = 0; lCol<lPictureWidth; lCol++, pMaskPtr++)
					{
						*pFramePtr = ((255-(*pMaskPtr))*(*pFramePtr) + (*pMaskPtr)*(*pPicturePtr))/255;
						pFramePtr++; pPicturePtr++;
						*pFramePtr = ((255-(*pMaskPtr))*(*pFramePtr) + (*pMaskPtr)*(*pPicturePtr))/255;
						pFramePtr++; pPicturePtr++;
						*pFramePtr = ((255-(*pMaskPtr))*(*pFramePtr) + (*pMaskPtr)*(*pPicturePtr))/255;
						pFramePtr++; pPicturePtr++;
						pFramePtr++; pPicturePtr++;
					}
					pFramePtr	+= lFramePitch;
					pPicturePtr	+= lPicturePitch;
					pMaskPtr	+= lMaskPitch;
				}
			}
			break;
		case CSP_BGR:
			{
				long lPictureWidth = min(oPictureSetting.m_lWidth, oFrameSetting.m_lWidth - lLeft);
				if (lLeft<0)
					lPictureWidth += lLeft;
				long lPictureHeight = min(oPictureSetting.m_lHeight, oFrameSetting.m_lHeight - lTop);
				if (lTop<0)
					lPictureHeight += lTop;

				LPBYTE pMaskPlane = NULL;
				pMask->get_Plane(0, &pMaskPlane);
				long lMaskStride;
				pMask->get_Stride(0, &lMaskStride);
			
				LPBYTE pFramePtr = NULL;
				pFrame->get_Plane(0, &pFramePtr);
				long lFrameStride;
				pFrame->get_Stride(0, &lFrameStride);

				LPBYTE pPicturePtr = NULL;
				pPicture->get_Plane(0, &pPicturePtr);
				long lPictureStride;
				pPicture->get_Stride(0, &lPictureStride);

				LPBYTE pMaskPtr = pMaskPlane;
				long lFramePitch = lFrameStride - 3*lPictureWidth;
				long lPicturePitch = lPictureStride - 3*lPictureWidth;
				long lMaskPitch = lMaskStride - lPictureWidth;
				if (lLeft>=0)
					pFramePtr += 3*lLeft;
				else
				{
					pPicturePtr -= 3*lLeft;
					pMaskPtr -= lLeft;
				}

				if (0!=oFrameSetting.m_lColorSpace & CSP_VFLIP)
				{
					lFramePitch		-= 2*lFrameStride;
					lPicturePitch	-= 2*lPictureStride;
					pFramePtr += (oFrameSetting.m_lHeight - 1)*lFrameStride;
					pPicturePtr += (oPictureSetting.m_lHeight - 1)*lPictureStride;
					if (lTop>=0)
						pFramePtr -= lTop*lFrameStride;
					else
						pPicturePtr += lTop*lPictureStride;
				}
				else
				{
					if (lTop>=0)
						pFramePtr += lTop*lFrameStride;
					else
						pPicturePtr -= lTop*lPictureStride;
				}
				if (0!=lMaskColorSpace & CSP_VFLIP)
				{
					lMaskPitch -= 2*lMaskStride;
					pMaskPtr += (oFrameSetting.m_lHeight-1)*lMaskStride;
					if (lTop<0)
						pMaskPtr += lTop*lMaskStride;
				}
				else
				{
					if (lTop<0)
						pMaskPtr -= lTop*lMaskStride;
				}
				
				for (long lRow = 0; lRow<lPictureHeight; lRow++)
				{
					for (long lCol = 0; lCol<lPictureWidth; lCol++, pMaskPtr++)
					{
						*pFramePtr = ((255-(*pMaskPtr))*(*pFramePtr) + (*pMaskPtr)*(*pPicturePtr))/255;
						pFramePtr++; pPicturePtr++;
						*pFramePtr = ((255-(*pMaskPtr))*(*pFramePtr) + (*pMaskPtr)*(*pPicturePtr))/255;
						pFramePtr++; pPicturePtr++;
						*pFramePtr = ((255-(*pMaskPtr))*(*pFramePtr) + (*pMaskPtr)*(*pPicturePtr))/255;
						pFramePtr++; pPicturePtr++;
					}
					pFramePtr	+= lFramePitch;
					pPicturePtr	+= lPicturePitch;
					pMaskPtr	+= lMaskPitch;
				}
			}
			break;
		default:
			break;
		}
	}
	static void DrawInner(IAVSUncompressedVideoFrame* pFrame, long lLeft, long lTop, IAVSUncompressedVideoFrame* pPicture)
	{
		SFrameSetting oFrameSetting;
		GetFrameProperties(pFrame, oFrameSetting);
		SFrameSetting oPictureSetting;
		GetFrameProperties(pPicture, oPictureSetting);

		switch (oFrameSetting.m_lColorSpace & CSP_COLOR_MASK)
		{
		case CSP_I420:
		case CSP_YV12:
			{
				if (lLeft<0)
					lLeft = -(abs(lLeft)>>1)<<1;
				else
					lLeft = (lLeft>>1)<<1;
				if (lTop<0)
					lTop = -(abs(lTop)>>1)<<1;
				else
					lTop = (lTop>>1)<<1;
				
				long lPictureWidth = (min(oPictureSetting.m_lWidth, oFrameSetting.m_lWidth - lLeft)>>1)<<1;
				if (lLeft<0)
					lPictureWidth += lLeft;
				long lPictureHeight = (min(oPictureSetting.m_lHeight, oFrameSetting.m_lHeight - lTop)>>1)<<1;
				if (lTop<0)
					lPictureHeight += lTop;

				{//YPlane
					LPBYTE pFrameY = NULL;
					pFrame->get_Plane(0, &pFrameY);
					long lFrameStrideY;
					pFrame->get_Stride(0, &lFrameStrideY);

					LPBYTE pPictureY = NULL;
					pPicture->get_Plane(0, &pPictureY);
					long lPictureStrideY;
					pPicture->get_Stride(0, &lPictureStrideY);

					long lFramePitch = lFrameStrideY;
					long lPicturePitch = lPictureStrideY;

					if (lLeft<0)
						pPictureY -= lLeft;
					else
						pFrameY += lLeft;

					if (0!=(oFrameSetting.m_lColorSpace & CSP_VFLIP))
					{
						lFramePitch		-= 2*lFrameStrideY;
						lPicturePitch	-= 2*lPictureStrideY;
						pPictureY += (oPictureSetting.m_lHeight - 1)*lPictureStrideY;
						pFrameY += (oFrameSetting.m_lHeight - 1)*lFrameStrideY;
						if (lTop<0)
							pPictureY += lTop*lPictureStrideY;
						else
							pFrameY -= lTop*lFrameStrideY;
					}
					else
					{
						if (lTop<0)
							pPictureY -= lTop*lPictureStrideY;
						else
							pFrameY += lTop*lFrameStrideY;
					}


					for (long lRow = 0; lRow<lPictureHeight; lRow++)
					{
						memcpy(pFrameY, pPictureY, lPictureWidth);
						pFrameY		+= lFramePitch;
						pPictureY	+= lPicturePitch;
					}
				}
				
				{//UVPlane
					LPBYTE pFrameU = NULL;
					LPBYTE pFrameV = NULL;
					pFrame->get_Plane(1, &pFrameU);
					pFrame->get_Plane(2, &pFrameV);
					long lFrameStrideU;
					long lFrameStrideV;
					pFrame->get_Stride(1, &lFrameStrideU);
					pFrame->get_Stride(2, &lFrameStrideV);

				
					LPBYTE pPictureU = NULL;
					LPBYTE pPictureV = NULL;
					pPicture->get_Plane(1, &pPictureU);
					pPicture->get_Plane(2, &pPictureV);

					long lPictureStrideU;
					long lPictureStrideV;
					pPicture->get_Stride(1, &lPictureStrideU);
					pPicture->get_Stride(2, &lPictureStrideV);

					long lFramePitchU = lFrameStrideU;
					long lFramePitchV = lFrameStrideV;
					long lPicturePitchU = lPictureStrideU;
					long lPicturePitchV = lPictureStrideV;
					if (lLeft<0)
					{
						pPictureU -= lLeft/2;
						pPictureV -= lLeft/2;
					}
					else
					{
						pFrameU += lLeft/2;
						pFrameV += lLeft/2;
					}
					if (0!=(oFrameSetting.m_lColorSpace & CSP_VFLIP))
					{
						lFramePitchU -= 2*lFrameStrideU;
						lFramePitchV -= 2*lFrameStrideV;
						lPicturePitchU -= 2*lPictureStrideU;
						lPicturePitchV -= 2*lPictureStrideV;

						pFrameU += lFrameStrideU*(oFrameSetting.m_lHeight/2 - 1);
						pFrameV += lFrameStrideV*(oFrameSetting.m_lHeight/2 - 1);

						pPictureU += (oPictureSetting.m_lHeight/2 - 1)*lPictureStrideU;
						pPictureV += (oPictureSetting.m_lHeight/2 - 1)*lPictureStrideV;

						if (lTop<0)
						{
							pPictureU += lTop/2*lPictureStrideU;
							pPictureV += lTop/2*lPictureStrideV;
						}
						else
						{
							pFrameU -= lTop/2*lFrameStrideU;
							pFrameV -= lTop/2*lFrameStrideV;
						}
					}
					else
					{
						if (lTop<0)
						{
							pPictureU -= lTop/2*lPictureStrideU;
							pPictureV -= lTop/2*lPictureStrideV;
						}
						else
						{
							pFrameU += lTop/2*lFrameStrideU;
							pFrameV += lTop/2*lFrameStrideV;
						}
					}
					for (long lRow = 0; lRow<lPictureHeight/2; lRow++)
					{
						memcpy(pFrameU, pPictureU, lPictureWidth/2);
						memcpy(pFrameV, pPictureV, lPictureWidth/2);
						pFrameU		+= lFramePitchU;
						pFrameV		+= lFramePitchV;
						pPictureU	+= lPicturePitchU;
						pPictureV	+= lPicturePitchV;
					}
				}
			}
			break;
		case CSP_YUY2:
			{
				if (lLeft<0)
					lLeft = -(abs(lLeft)>>1)<<1;
				else
					lLeft = (lLeft>>1)<<1;
				if (lTop<0)
					lTop = -(abs(lTop)>>1)<<1;
				else
					lTop = (lTop>>1)<<1;

				long lPictureWidth = (min(oPictureSetting.m_lWidth, oFrameSetting.m_lWidth - lLeft)>>1)<<1;
				if (lLeft<0)
					lPictureWidth += lLeft;
				long lPictureHeight = (min(oPictureSetting.m_lHeight, oFrameSetting.m_lHeight - lTop)>>1)<<1;
				if (lTop<0)
					lPictureHeight += lTop;

				LPBYTE pFramePtr = NULL;
				pFrame->get_Plane(0, &pFramePtr);
				long lFrameStride;
				pFrame->get_Stride(0, &lFrameStride);

				LPBYTE pPicturePtr = NULL;
				pPicture->get_Plane(0, &pPicturePtr);
				long lPictureStride;
				pPicture->get_Stride(0, &lPictureStride);

				long lFramePitch = lFrameStride;
				long lPicturePitch = lPictureStride;
				if (lLeft>=0)
					pFramePtr += 2*lLeft;
				else
					pPicturePtr -= 2*lLeft;

				if (0!=oFrameSetting.m_lColorSpace & CSP_VFLIP)
				{
					lFramePitch		-= 2*lFrameStride;
					lPicturePitch	-= 2*lPictureStride;
					pFramePtr += (oFrameSetting.m_lHeight - 1)*lFrameStride;
					pPicturePtr += (oPictureSetting.m_lHeight - 1)*lPictureStride;
					if (lTop>=0)
						pFramePtr -= lTop * lFrameStride;
					else
						pPicturePtr += lTop * lPictureStride;
				}
				else
				{
					if (lTop>=0)
						pFramePtr += lTop * lFrameStride;
					else
						pPicturePtr -= lTop * lPictureStride;
				}
				for (long lRow = 0; lRow<lPictureHeight; lRow++)
				{
					memcpy(pFramePtr, pPicturePtr, 2*lPictureWidth);
					pFramePtr	+= lFramePitch;
					pPicturePtr	+= lPicturePitch;
				}
			}
			break;
		case CSP_BGRA:
			{
				long lPictureWidth = min(oPictureSetting.m_lWidth, oFrameSetting.m_lWidth - lLeft);
				if (lLeft<0)
					lPictureWidth += lLeft;
				long lPictureHeight = min(oPictureSetting.m_lHeight, oFrameSetting.m_lHeight - lTop);
				if (lTop<0)
					lPictureHeight += lTop;

			
				LPBYTE pFramePtr = NULL;
				pFrame->get_Plane(0, &pFramePtr);
				long lFrameStride;
				pFrame->get_Stride(0, &lFrameStride);

				LPBYTE pPicturePtr = NULL;
				pPicture->get_Plane(0, &pPicturePtr);
				long lPictureStride;
				pPicture->get_Stride(0, &lPictureStride);

				long lFramePitch = lFrameStride;
				long lPicturePitch = lPictureStride;
				if (lLeft>=0)
					pFramePtr += 4*lLeft;
				else
					pPicturePtr -= 4*lLeft;

				if (0!=oFrameSetting.m_lColorSpace & CSP_VFLIP)
				{
					lFramePitch		-= 2*lFrameStride;
					lPicturePitch	-= 2*lPictureStride;
					pFramePtr += (oFrameSetting.m_lHeight - 1)*lFrameStride;
					pPicturePtr += (oPictureSetting.m_lHeight - 1)*lPictureStride;
					if (lTop>=0)
						pFramePtr -= lTop*lFrameStride;
					else
						pPicturePtr += lTop*lPictureStride;
				}
				else
				{
					if (lTop>=0)
						pFramePtr += lTop*lFrameStride;
					else
						pPicturePtr -= lTop*lPictureStride;
				}
				for (long lRow = 0; lRow<lPictureHeight; lRow++)
				{
					memcpy(pFramePtr, pPicturePtr, 4*lPictureWidth);
					pFramePtr	+= lFramePitch;
					pPicturePtr	+= lPicturePitch;
				}
			}
			break;
		case CSP_BGR:
			{
				long lPictureWidth = min(oPictureSetting.m_lWidth, oFrameSetting.m_lWidth - lLeft);
				if (lLeft<0)
					lPictureWidth += lLeft;
				long lPictureHeight = min(oPictureSetting.m_lHeight, oFrameSetting.m_lHeight - lTop);
				if (lTop<0)
					lPictureHeight += lTop;

				LPBYTE pFramePtr = NULL;
				pFrame->get_Plane(0, &pFramePtr);
				long lFrameStride;
				pFrame->get_Stride(0, &lFrameStride);

				LPBYTE pPicturePtr = NULL;
				pPicture->get_Plane(0, &pPicturePtr);
				long lPictureStride;
				pPicture->get_Stride(0, &lPictureStride);

				long lFramePitch = lFrameStride;
				long lPicturePitch = lPictureStride;
				if (lLeft>=0)
					pFramePtr += 3*lLeft;
				else
					pPicturePtr -= 3*lLeft;

				if (0!=oFrameSetting.m_lColorSpace & CSP_VFLIP)
				{
					lFramePitch		-= 2*lFrameStride;
					lPicturePitch	-= 2*lPictureStride;
					pFramePtr += (oFrameSetting.m_lHeight - 1)*lFrameStride;
					pPicturePtr += (oPictureSetting.m_lHeight - 1)*lPictureStride;
					if (lTop>=0)
						pFramePtr -= lTop*lFrameStride;
					else
						pPicturePtr += lTop*lPictureStride;
				}
				else
				{
					if (lTop>=0)
						pFramePtr += lTop*lFrameStride;
					else
						pPicturePtr -= lTop*lPictureStride;
				}
				for (long lRow = 0; lRow<lPictureHeight; lRow++)
				{
					memcpy(pFramePtr, pPicturePtr, 3*lPictureWidth);
					pFramePtr	+= lFramePitch;
					pPicturePtr	+= lPicturePitch;
				}
			}
			break;
		default:
			break;
		}
	}
	static void DrawSimpleInner(IAVSUncompressedVideoFrame* pFrame, long lLeft, long lTop, IAVSUncompressedVideoFrame* pPicture, long lColorKey)
	{
		SFrameSetting oFrameSetting;
		GetFrameProperties(pFrame, oFrameSetting);
		SFrameSetting oPictureSetting;
		GetFrameProperties(pPicture, oPictureSetting);

		switch (oFrameSetting.m_lColorSpace & CSP_COLOR_MASK)
		{
		case CSP_I420:
		case CSP_YV12:
			{
				BYTE nYColorKey = (lColorKey>>16)&0xFF;
				BYTE nUColorKey = (lColorKey>>8)&0xFF;
				BYTE nVColorKey = (lColorKey)&0xFF;
				if (CSP_YV12 == (oFrameSetting.m_lColorSpace & CSP_COLOR_MASK))
				{
					BYTE nTemp = nUColorKey;
					nUColorKey = nVColorKey;
					nVColorKey = nTemp;
				}
				lLeft = (lLeft>>1)<<1;
				lTop = (lTop>>1)<<1;

				long lPictureWidth = (min(oPictureSetting.m_lWidth, oFrameSetting.m_lWidth - lLeft)>>1)<<1;
				long lPictureHeight = (min(oPictureSetting.m_lHeight, oFrameSetting.m_lHeight - lTop)>>1)<<1;

				LPBYTE pFrameYFirst = NULL;
				pFrame->get_Plane(0, &pFrameYFirst);
				long lFrameStrideY;
				pFrame->get_Stride(0, &lFrameStrideY);
				LPBYTE pFrameYSecond = NULL;

				LPBYTE pFrameU = NULL;
				LPBYTE pFrameV = NULL;
				pFrame->get_Plane(1, &pFrameU);
				pFrame->get_Plane(2, &pFrameV);
				long lFrameStrideU;
				long lFrameStrideV;
				pFrame->get_Stride(1, &lFrameStrideU);
				pFrame->get_Stride(2, &lFrameStrideV);

				LPBYTE pPictureYFirst = NULL;
				pPicture->get_Plane(0, &pPictureYFirst);
				long lPictureStrideY;
				pPicture->get_Stride(0, &lPictureStrideY);
				LPBYTE pPictureU = NULL;
				LPBYTE pPictureV = NULL;
				pPicture->get_Plane(1, &pPictureU);
				pPicture->get_Plane(2, &pPictureV);
				long lPictureStrideU;
				long lPictureStrideV;
				pPicture->get_Stride(1, &lPictureStrideU);
				pPicture->get_Stride(2, &lPictureStrideV);
				LPBYTE pPictureYSecond = pPictureYFirst + lPictureStrideY;


				long lFramePitchY	= 2*lFrameStrideY - lPictureWidth;
				long lFramePitchU	= lFrameStrideU - lPictureWidth/2;
				long lFramePitchV	= lFrameStrideV - lPictureWidth/2;

				long lPicturePitchY	= 2*lPictureStrideY - lPictureWidth;
				long lPicturePitchU = lPictureStrideU - lPictureWidth/2;
				long lPicturePitchV = lPictureStrideV - lPictureWidth/2;
				if (0!=oFrameSetting.m_lColorSpace & CSP_VFLIP)
				{
					lFramePitchY	-= 4*lFrameStrideY;
					lFramePitchU	-= 2*lFrameStrideU;
					lFramePitchV	-= 2*lFrameStrideV;

					lPicturePitchY	-= 4*lPictureStrideY;
					lPicturePitchU	-= 2*lPictureStrideU;
					lPicturePitchV	-= 2*lPictureStrideV;

					pFrameYFirst += lLeft + (oFrameSetting.m_lHeight - 1 - lTop)*lFrameStrideY;
					pFrameYSecond = pFrameYFirst - lFrameStrideY;
					pFrameU += lLeft/2 + lFrameStrideU*(oFrameSetting.m_lHeight/2 - 1 - lTop/2);
					pFrameV += lLeft/2 + lFrameStrideV*(oFrameSetting.m_lHeight/2 - 1 - lTop/2);

					pPictureYFirst += (oPictureSetting.m_lHeight - 1)*lPictureStrideY;
					pPictureYSecond = pPictureYFirst - lPictureStrideY;
					pPictureU += (oPictureSetting.m_lHeight/2 - 1)*lPictureStrideU;
					pPictureV += (oPictureSetting.m_lHeight/2 - 1)*lPictureStrideV;
				}
				else
				{
					pFrameYFirst += lLeft + lTop*lFrameStrideY;
					pFrameYSecond = pFrameYFirst + lFrameStrideY;
					pFrameU += lLeft/2 + lFrameStrideU*lTop/2;
					pFrameV += lLeft/2 + lFrameStrideV*lTop/2;
				}

				for (long lRow = 0; lRow<lPictureHeight/2; lRow++)
				{
					for (long lCol = 0; lCol<lPictureWidth/2; lCol++)
					{
						if ((nUColorKey!=(*pPictureU))||(nVColorKey!=(*pPictureV))||
							( (nYColorKey!=(*pPictureYFirst))&&(nYColorKey!=(*(pPictureYFirst+1)))&&
							  (nYColorKey!=(*pPictureYSecond))&&(nYColorKey!=(*(pPictureYSecond+1)))))
						{
							*pFrameYFirst	= *pPictureYFirst;
							pFrameYFirst++; pPictureYFirst++;
							*pFrameYFirst	= *pPictureYFirst;
							pFrameYFirst++; pPictureYFirst++;

							*pFrameYSecond	= *pPictureYSecond;
							pFrameYSecond++; pPictureYSecond++;
							*pFrameYSecond	= *pPictureYSecond;
							pFrameYSecond++; pPictureYSecond++;

							*pFrameU = *pPictureU;
							pFrameU++; pPictureU++;
							*pFrameV = *pPictureV;
							pFrameV++; pPictureV++;
						}
						else
						{
							pFrameYFirst+=2; pPictureYFirst+=2;
							pFrameYSecond+=2; pPictureYSecond+=2;
							pFrameU++; pPictureU++;
							pFrameV++; pPictureV++;
						}						
					}
					pFrameYFirst	+= lFramePitchY;
					pFrameYSecond	+= lFramePitchY;
					pFrameU			+= lFramePitchU;
					pFrameV			+= lFramePitchV;
					pPictureYFirst	+= lPicturePitchY;
					pPictureYSecond	+= lPicturePitchY;
					pPictureU		+= lPicturePitchU;
					pPictureV		+= lPicturePitchV;
				}
			}
			break;
		case CSP_YUY2:
			{
				lLeft = (lLeft>>1)<<1;
				lTop = (lTop>>1)<<1;

				long lPictureWidth = (min(oPictureSetting.m_lWidth, oFrameSetting.m_lWidth - lLeft)>>1)<<1;
				long lPictureHeight = (min(oPictureSetting.m_lHeight, oFrameSetting.m_lHeight - lTop)>>1)<<1;

				LPBYTE pFramePtr = NULL;
				pFrame->get_Plane(0, &pFramePtr);
				long lFrameStride;
				pFrame->get_Stride(0, &lFrameStride);

				LPBYTE pPicturePtr = NULL;
				pPicture->get_Plane(0, &pPicturePtr);
				long lPictureStride;
				pPicture->get_Stride(0, &lPictureStride);

				long lFramePitch = lFrameStride - 2*lPictureWidth;
				long lPicturePitch = lPictureStride - 2*lPictureWidth;
				if (0!=oFrameSetting.m_lColorSpace & CSP_VFLIP)
				{
					lFramePitch		-= 2*lFrameStride;
					lPicturePitch	-= 2*lPictureStride;
					pFramePtr += 2*lLeft + (oFrameSetting.m_lHeight - 1 - lTop)*lFrameStride;
					pPicturePtr += (oPictureSetting.m_lHeight - 1)*lPictureStride;
				}
				else
				{
					pFramePtr += 2*lLeft + lTop*lFrameStride;
				}
				for (long lRow = 0; lRow<lPictureHeight; lRow++)
				{
					for (long lCol = 0; lCol<lPictureWidth; lCol++)
					{
						*pFramePtr = *pPicturePtr;
						pFramePtr++; pPicturePtr++;
						*pFramePtr = *pPicturePtr;
						pFramePtr++; pPicturePtr++;
					}
					pFramePtr	+= lFramePitch;
					pPicturePtr	+= lPicturePitch;
				}
			}
			break;
		case CSP_BGRA:
			{
				long lPictureWidth = min(oPictureSetting.m_lWidth, oFrameSetting.m_lWidth - lLeft);
				long lPictureHeight = min(oPictureSetting.m_lHeight, oFrameSetting.m_lHeight - lTop);

				LPBYTE pFramePtr = NULL;
				pFrame->get_Plane(0, &pFramePtr);
				long lFrameStride;
				pFrame->get_Stride(0, &lFrameStride);

				LPBYTE pPicturePtr = NULL;
				pPicture->get_Plane(0, &pPicturePtr);
				long lPictureStride;
				pPicture->get_Stride(0, &lPictureStride);

				long lFramePitch = lFrameStride - 4*lPictureWidth;
				long lPicturePitch = lPictureStride - 4*lPictureWidth;
				if (0!=oFrameSetting.m_lColorSpace & CSP_VFLIP)
				{
					lFramePitch		-= 2*lFrameStride;
					lPicturePitch	-= 2*lPictureStride;
					pFramePtr += 4*lLeft + (oFrameSetting.m_lHeight - 1 - lTop)*lFrameStride;
					pPicturePtr += (oPictureSetting.m_lHeight - 1)*lPictureStride;
				}
				else
				{
					pFramePtr += 4*lLeft + lTop*lFrameStride;
				}
				for (long lRow = 0; lRow<lPictureHeight; lRow++)
				{
					for (long lCol = 0; lCol<lPictureWidth; lCol++, pFramePtr+=4, pPicturePtr+=4)
					{
						if (((*(LPDWORD)pPicturePtr)&0xFFFFFF)!=lColorKey)
							memcpy(pFramePtr, pPicturePtr, 3);
					}
					pFramePtr	+= lFramePitch;
					pPicturePtr	+= lPicturePitch;
				}
			}
			break;
		case CSP_BGR:
			{
				long lPictureWidth = min(oPictureSetting.m_lWidth, oFrameSetting.m_lWidth - lLeft);
				long lPictureHeight = min(oPictureSetting.m_lHeight, oFrameSetting.m_lHeight - lTop);

				LPBYTE pFramePtr = NULL;
				pFrame->get_Plane(0, &pFramePtr);
				long lFrameStride;
				pFrame->get_Stride(0, &lFrameStride);

				LPBYTE pPicturePtr = NULL;
				pPicture->get_Plane(0, &pPicturePtr);
				long lPictureStride;
				pPicture->get_Stride(0, &lPictureStride);

				long lFramePitch = lFrameStride - 3*lPictureWidth;
				long lPicturePitch = lPictureStride - 3*lPictureWidth;
				if (0!=oFrameSetting.m_lColorSpace & CSP_VFLIP)
				{
					lFramePitch		-= 2*lFrameStride;
					lPicturePitch	-= 2*lPictureStride;
					pFramePtr += 3*lLeft + (oFrameSetting.m_lHeight - 1 - lTop)*lFrameStride;
					pPicturePtr += (oPictureSetting.m_lHeight - 1)*lPictureStride;
				}
				else
				{
					pFramePtr += 3*lLeft + lTop*lFrameStride;
				}
				for (long lRow = 0; lRow<lPictureHeight; lRow++)
				{
					for (long lCol = 0; lCol<lPictureWidth; lCol++, pFramePtr+=3, pPicturePtr+=3)
					{
						if (((*(LPDWORD)pPicturePtr)&0xFFFFFF)!=lColorKey)
							memcpy(pFramePtr, pPicturePtr, 3);
					}
					pFramePtr	+= lFramePitch;
					pPicturePtr	+= lPicturePitch;
				}
			}
			break;
		default:
			break;
		}
	}
	
	static BOOL IsEqualColorSpace(long lColorSpace1, long lColorSpace2)
	{
		lColorSpace1 &= CSP_COLOR_MASK;
		lColorSpace2 &= CSP_COLOR_MASK;
		if (((CSP_BGRA==lColorSpace1)||(CSP_BGR==lColorSpace1))&&
			((CSP_BGRA==lColorSpace2)||(CSP_BGR==lColorSpace2)))
			return TRUE;

		if (((CSP_I420==lColorSpace1)||(CSP_YV12==lColorSpace1)||(CSP_YUY2==lColorSpace1))&&
			((CSP_I420==lColorSpace2)||(CSP_YV12==lColorSpace2)||(CSP_YUY2==lColorSpace2)))
			return TRUE;
		return FALSE;
	}
	static BOOL IsYUVColorSpace(long lColorSpace)
	{
		lColorSpace &= CSP_COLOR_MASK;
		return  ((CSP_I420==lColorSpace)||(CSP_YV12==lColorSpace)||(CSP_YUY2==lColorSpace));
	}
	static BOOL IsRGBColorSpace(long lColorSpace)
	{
		lColorSpace &= CSP_COLOR_MASK;
		return  ((CSP_BGRA==lColorSpace)||(CSP_BGR==lColorSpace));
	}

	long ConvertColorKey(long lColorKey, long lSrcColorSpace, long lDstColorSpace)
	{
		if (IsEqualColorSpace(lSrcColorSpace, lDstColorSpace))
			return lColorKey;
		if (IsYUVColorSpace(lSrcColorSpace))
		{
			IAVSVideoFrameTransform *pTransform = NULL;
			CoCreateInstance(__uuidof(CAVSVideoFrameTransform), NULL, CLSCTX_ALL, __uuidof(IAVSVideoFrameTransform), (void**)&pTransform);
			if (NULL==pTransform)
				return S_FALSE;
			MediaFormat::IAVSVideoFormat *pFormat = NULL;
			CoCreateInstance(MediaFormat::CLSID_CAVSVideoFormat, NULL, CLSCTX_ALL, MediaFormat::IID_IAVSVideoFormat, (void**)&pFormat);
			if (NULL==pFormat)
			{
				pTransform->Release();
				return S_FALSE;
			}
			pTransform->put_DeinterlaceEnabled(FALSE);
			pTransform->put_CropScale(0);

			pFormat->put_ColorSpace(CSP_BGRA);
			pFormat->put_Width(2);
			pFormat->put_Height(2);
			pFormat->put_AspectRatioX(0);
			pFormat->put_AspectRatioY(0);
			pTransform->SetVideoFormat(pFormat);
			RELEASEINTERFACE(pFormat);
			
			IAVSUncompressedVideoFrame *pTestFrameSrc = NULL;
			CoCreateInstance(__uuidof(CAVSUncompressedVideoFrame), NULL, CLSCTX_ALL, __uuidof(IAVSUncompressedVideoFrame), (void**)&pTestFrameSrc);
			pTestFrameSrc->put_ColorSpace(CSP_I420);
			pTestFrameSrc->put_Width(2);
			pTestFrameSrc->put_Height(2);
			pTestFrameSrc->put_AspectRatioX(0);
			pTestFrameSrc->put_AspectRatioY(0);
			pTestFrameSrc->SetDefaultStrides();
			pTestFrameSrc->AllocateBuffer(-1);
			BYTE nYColor = (lColorKey>>16) & 0xFF;
			BYTE nUColor = (lColorKey>>8) & 0xFF;
			BYTE nVColor = lColorKey & 0xFF;
			
			LPBYTE pBuffer = NULL;
			pTestFrameSrc->get_Plane(0, &pBuffer);
			*pBuffer = nYColor; pBuffer++;
			*pBuffer = nYColor; pBuffer++;
			*pBuffer = nYColor; pBuffer++;
			*pBuffer = nYColor; pBuffer++;
			pTestFrameSrc->get_Plane(1, &pBuffer);
			*pBuffer = nUColor; pBuffer++;
			*pBuffer = nUColor; pBuffer++;
			pTestFrameSrc->get_Plane(2, &pBuffer);
			*pBuffer = nVColor; pBuffer++;
			*pBuffer = nVColor; pBuffer++;
			IUnknown *punkTestFrameSrc = NULL;
			pTestFrameSrc->QueryInterface(__uuidof(IUnknown), (void**)&punkTestFrameSrc);
			pTestFrameSrc->Release();
			
			IUnknown* punkTestFrameDst = NULL;
			pTransform->TransformFrame(punkTestFrameSrc, &punkTestFrameDst);
			RELEASEINTERFACE(pTransform);
			RELEASEINTERFACE(punkTestFrameSrc);
			if (NULL==punkTestFrameDst)
				return lColorKey;
			IAVSUncompressedVideoFrame *pTestFrameDst = NULL;
			punkTestFrameDst->QueryInterface(__uuidof(IAVSUncompressedVideoFrame), (void**)&pTestFrameDst);
			punkTestFrameDst->Release();

			pTestFrameDst->get_Plane(0,&pBuffer);
			lColorKey =		(*pBuffer)<<16; pBuffer++;
			lColorKey |=	(*pBuffer)<<8; pBuffer++;
			lColorKey |=	*pBuffer;
			pTestFrameDst->Release();
			return lColorKey;
		}
		else
		{
			IAVSVideoFrameTransform *pTransform = NULL;
			CoCreateInstance(__uuidof(CAVSVideoFrameTransform), NULL, CLSCTX_ALL, __uuidof(IAVSVideoFrameTransform), (void**)&pTransform);
			if (NULL==pTransform)
				return S_FALSE;
			MediaFormat::IAVSVideoFormat *pFormat = NULL;
			CoCreateInstance(MediaFormat::CLSID_CAVSVideoFormat, NULL, CLSCTX_ALL, MediaFormat::IID_IAVSVideoFormat, (void**)&pFormat);
			if (NULL==pFormat)
			{
				pTransform->Release();
				return lColorKey;
			}
			pTransform->put_DeinterlaceEnabled(FALSE);
			pTransform->put_CropScale(0);

			pFormat->put_ColorSpace(CSP_YUY2);
			pFormat->put_Width(2);
			pFormat->put_Height(2);
			pFormat->put_AspectRatioX(0);
			pFormat->put_AspectRatioY(0);
			pTransform->SetVideoFormat(pFormat);
			RELEASEINTERFACE(pFormat);
			
			IAVSUncompressedVideoFrame *pTestFrameSrc = NULL;
			CoCreateInstance(__uuidof(CAVSUncompressedVideoFrame), NULL, CLSCTX_ALL, __uuidof(IAVSUncompressedVideoFrame), (void**)&pTestFrameSrc);
			if (NULL==pTestFrameSrc)
			{
				pTransform->Release();
				return lColorKey;
			}
			pTestFrameSrc->put_ColorSpace(CSP_BGR);
			pTestFrameSrc->put_Width(2);
			pTestFrameSrc->put_Height(2);
			pTestFrameSrc->put_AspectRatioX(0);
			pTestFrameSrc->put_AspectRatioY(0);
			pTestFrameSrc->SetDefaultStrides();
			pTestFrameSrc->AllocateBuffer(-1);
			BYTE nRColor = (lColorKey>>16) & 0xFF;
			BYTE nGColor = (lColorKey>>8) & 0xFF;
			BYTE nBColor = lColorKey & 0xFF;
			
			LPBYTE pBuffer = NULL;
			pTestFrameSrc->get_Plane(0, &pBuffer);
			*pBuffer = nBColor; pBuffer++;
			*pBuffer = nGColor; pBuffer++;
			*pBuffer = nRColor; pBuffer++;

			*pBuffer = nBColor; pBuffer++;
			*pBuffer = nGColor; pBuffer++;
			*pBuffer = nRColor; pBuffer++;

			*pBuffer = nBColor; pBuffer++;
			*pBuffer = nGColor; pBuffer++;
			*pBuffer = nRColor; pBuffer++;

			*pBuffer = nBColor; pBuffer++;
			*pBuffer = nGColor; pBuffer++;
			*pBuffer = nRColor; pBuffer++;
			IUnknown *punkTestFrameSrc = NULL;
			pTestFrameSrc->QueryInterface(__uuidof(IUnknown), (void**)&punkTestFrameSrc);
			pTestFrameSrc->Release();
			
			IUnknown* punkTestFrameDst = NULL;
			pTransform->TransformFrame(punkTestFrameSrc, &punkTestFrameDst);
			RELEASEINTERFACE(pTransform);
			RELEASEINTERFACE(punkTestFrameSrc);
			if (NULL==punkTestFrameDst)
				return lColorKey;
			IAVSUncompressedVideoFrame *pTestFrameDst = NULL;
			punkTestFrameDst->QueryInterface(__uuidof(IAVSUncompressedVideoFrame), (void**)&pTestFrameDst);
			punkTestFrameDst->Release();

			pTestFrameDst->get_Plane(0,&pBuffer);
			lColorKey =		(*pBuffer)<<16; pBuffer++;
			lColorKey |=	(*pBuffer)<<8; pBuffer++; pBuffer++;
			lColorKey |=	*pBuffer;
			pTestFrameDst->Release();
		}	
		return lColorKey;
	}
};

