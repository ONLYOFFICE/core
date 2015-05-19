#ifndef __XML_COMM_
#define __XML_COMM_

#include "..//CommandManager.h"
/************************************************************************/
/* XML COMMAND   -----   ID = 109                                    */
/************************************************************************/
#include <ImageTransformsCoreBGRA.h>

class CXMLCommand : public CCommand
{
public:
	CXMLCommand(IAppManager* app_manager):CCommand(app_manager)
	{
		AddParameter(_T("PhotoEffect"), _T("String"), CString(_T("oo")));
				
		m_pThumbnailImage = NULL;
		long lThWidth = 0; 
		long lThHeight = 0;
		const BYTE* pBuffer = GetAppManager()->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::FAST, lThWidth, lThHeight);

		CoCreateInstance(MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&m_pThumbnailImage);
		if (NULL == m_pThumbnailImage)
			return;

		//CoCreateInstance(ImageStudio3::CLSID_ImageTransforms, NULL, CLSCTX_ALL, ImageStudio3::IID_IImageTransforms, (void**)&m_pTransforms);
		//if (NULL == m_pTransforms)
		//	return;

		m_pThumbnailImage->put_ColorSpace(64); // BGRA
		m_pThumbnailImage->put_Width(lThWidth);
		m_pThumbnailImage->put_Height(lThHeight);
		m_pThumbnailImage->SetDefaultStrides();
		m_pThumbnailImage->AllocateBuffer(-1);

		BYTE* pDstBuffer = NULL;
		m_pThumbnailImage->get_Buffer(&pDstBuffer);
		memcpy(pDstBuffer,pBuffer,4 * lThWidth * lThHeight);
		this->BGRA_Flip(pDstBuffer, lThWidth, lThHeight);

		m_pBitmapThumbnail = new Bitmap(lThWidth, lThHeight, PixelFormat32bppARGB);

		BitmapData oBitmapData; 
		if (Gdiplus::Ok != m_pBitmapThumbnail->LockBits (NULL, ImageLockModeRead | ImageLockModeWrite, 
			PixelFormat32bppARGB, &oBitmapData))
				return;
		BYTE* pData = (BYTE*)oBitmapData.Scan0;
		memcpy(pData, pDstBuffer, 4 * lThWidth * lThHeight);		

		m_pBitmapThumbnail->UnlockBits(&oBitmapData);

		m_pTransforms = GetAppManager()->GetImageStudio();
		
	}
	virtual ~CXMLCommand() 
	{
		RELEASEOBJECT(m_pBitmapThumbnail);
		RELEASEINTERFACE(m_pThumbnailImage);
		
		m_pTransforms = NULL;
	}
public:

	bool BGRA_Flip(BYTE* pBGRA, int nWidth, int nHeight)
	{
		if (NULL == pBGRA)
			return false;

		int nLineSize = 4*nWidth;

		BYTE* pBGRALineCopy = new BYTE[nLineSize];
		if (NULL == pBGRALineCopy)
			return false;

		BYTE* pBGRALineTop = pBGRA;
		BYTE* pBGRALineBottom = pBGRA + nLineSize*(nHeight - 1);

		for (int nY = 0; nY < nHeight/2; ++nY)
		{
			memcpy(pBGRALineCopy, pBGRALineTop, nLineSize);
			memcpy(pBGRALineTop, pBGRALineBottom, nLineSize);
			memcpy(pBGRALineBottom, pBGRALineCopy, nLineSize);

			pBGRALineTop += nLineSize;
			pBGRALineBottom -= nLineSize;
		}

		delete[] pBGRALineCopy;

		return true;
	}

	void ResizeBYTE(const BYTE* pSource, BYTE* pResult, LONG lSrcW, LONG lSrcH, LONG lDstW, LONG lDstH)
	{
		IppiSize sizeSrc = {lSrcW, lSrcH};
		IppiSize sizeDst = {lDstW, lDstH};
		IppiRect srcRect = {0, 0, lSrcW, lSrcH};
		double dScale = ((double)(lDstW)) / lSrcW;

		ippiResize_8u_C4R(pSource, sizeSrc, lSrcW*4, 
			srcRect, pResult, 4*lDstW, sizeDst, dScale, dScale, IPPI_INTER_LINEAR);
	}

	void GetTransform(MediaCore::IAVSUncompressedVideoFrame*& pFrame)
	{
		VARIANT var;
		m_pTransforms->GetResult(0, &var);

		if (NULL != var.punkVal)
		{
			var.punkVal->QueryInterface(MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pFrame);
			RELEASEINTERFACE((var.punkVal));
		}
	}

	virtual void OnChangeParameter(int paramInd) 
	{
		if (NULL == m_pThumbnailImage)
			return;

		if ((paramInd==0 && m_params.GetCount()>0))
		{
			m_transform_string = CString(m_params[0]->m_value.bstrVal);

			if (NULL == m_pBitmapThumbnail)
				return;

			ApplyEffect(m_pThumbnailImage);
			
			if (!m_transform_string.IsEmpty())
			{
				SetInvalidateFlag(INVALID_GRAPHICS);
			}
		}
	}

	void ApplyEffect(MediaCore::IAVSUncompressedVideoFrame*& pFrame, bool bFlag = false)
	{
		if (NULL == pFrame)
			return;
		
		BSTR bstrTr = m_transform_string.AllocSysString();
		
		long lSrcW = 0; long lSrcH = 0;
		pFrame->get_Width(&lSrcW);
		pFrame->get_Height(&lSrcH);
		
		MediaCore::IAVSUncompressedVideoFrame* pResult = NULL;
		VARIANT_BOOL vbRes = VARIANT_FALSE;
		
		VARIANT var;
		var.vt = VT_UNKNOWN;
		var.punkVal = (IUnknown*)pFrame;
		
		if (!m_transform_string.IsEmpty())
		{
			m_pTransforms->SetSource(0, var);
			if (bFlag)
			{
				RELEASEINTERFACE(pFrame);
			}

			m_pTransforms->SetXml(bstrTr, &vbRes);
			SysFreeString(bstrTr);
			m_pTransforms->Transform(&vbRes);
			GetTransform(pResult);

			if (NULL != pResult)
			{
				long lW = 0; long lH = 0;
				pResult->get_Width(&lW); 
				pResult->get_Height(&lH);

				if (lW == lSrcW && lH == lSrcH)
				{
					if (!bFlag)
					{
						BYTE* pBuf = NULL;
						pResult->get_Buffer(&pBuf);
						
						BitmapData oBitmapData; 
						if (Gdiplus::Ok != m_pBitmapThumbnail->LockBits (NULL, ImageLockModeRead | ImageLockModeWrite, 
							PixelFormat32bppARGB, &oBitmapData))
								return;
						BYTE* pData = (BYTE*)oBitmapData.Scan0;
						memcpy(pData, pBuf, 4 * lSrcW * lSrcH);

						m_pBitmapThumbnail->UnlockBits(&oBitmapData);
					}
				}
				if (!bFlag)
				{
					RELEASEINTERFACE(pResult);
				}
				else
				{
					pFrame = pResult;
				}
			}
		}
	}

	virtual void DrawOnGraphics(Gdiplus::Graphics* pGr, const CRect& rect)
	{
		Gdiplus::Status eStatus;
		Gdiplus::SolidBrush oBrush(GetAppManager()->GetView()->GetBackgroundColor());
		pGr->FillRectangle(&oBrush,GetAppManager()->GetView()->GetImageRect());
		if (NULL != m_pBitmapThumbnail)
		{
			eStatus = pGr->DrawImage(m_pBitmapThumbnail, GetAppManager()->GetView()->GetImageRect());
		}
	}

	virtual bool DrawOnLayer()	
	{	
		MediaCore::IAVSUncompressedVideoFrame* pNewFrame = NULL;
		CoCreateInstance(MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pNewFrame);
		if (NULL == pNewFrame)
			return false;

		long lWidth = 0; long lHeight = 0;
		const BYTE* pBuffer = GetAppManager()->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::SLOW, lWidth, lHeight);

		pNewFrame->put_ColorSpace(64); // BGRA
		pNewFrame->put_Width(lWidth);
		pNewFrame->put_Height(lHeight);
		pNewFrame->SetDefaultStrides();
		pNewFrame->AllocateBuffer(-1);

		BYTE* pDstBuffer = NULL;
		pNewFrame->get_Buffer(&pDstBuffer);
		memcpy(pDstBuffer, pBuffer, 4 * lWidth * lHeight);

		ApplyEffect(pNewFrame, true);

		pNewFrame->get_Buffer(&pDstBuffer);
		
		BYTE* pNew = new BYTE[4 * lWidth * lHeight];
		for (long rowInd = 0; rowInd < lHeight; ++rowInd)
			memcpy(pNew+4*lWidth*rowInd, pDstBuffer+4*lWidth*(lHeight-1-rowInd), 4*lWidth);
		//this->BGRA_Flip(pDstBuffer, lWidth, lHeight);

		GetAppManager()->GetLayersManager()->GetCurrentLayer()->StartHistoryBlock();
		GetAppManager()->GetLayersManager()->GetCurrentLayer()->ChangeAllLayer(/*pDstBuffer*/pNew, lWidth, lHeight);
		GetAppManager()->GetLayersManager()->GetCurrentLayer()->StopHistoryBlock(_T("Effect-XML"));

		GetAppManager()->GetLayersManager()->GetCurrentLayer()->ChangeImage(ILayer::SLOW);

		RELEASEINTERFACE(pNewFrame);

		return true;
	}

	void SaveToFile(BYTE* pScan0, long lWidth, long lHeight)
	{
		MediaCore::IAVSUncompressedVideoFrame* pFrame = NULL;
		ImageStudio3::IImageTransforms* pTransform = NULL;
		
		CoCreateInstance(MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pFrame);
		if (NULL == pFrame)
			return;

		CoCreateInstance(ImageStudio3::CLSID_ImageTransforms, NULL, CLSCTX_ALL, ImageStudio3::IID_IImageTransforms, (void**)&pTransform);
		if (NULL == pTransform)
			return;

		pFrame->put_ColorSpace(64); // BGRA
		pFrame->put_Width(lWidth);
		pFrame->put_Height(lHeight);
		pFrame->SetDefaultStrides();
		pFrame->AllocateBuffer(-1);

		BYTE* pBuffer = NULL;
		pFrame->get_Buffer(&pBuffer);
		memcpy(pBuffer, pScan0, 4 * lWidth * lHeight);

		VARIANT var;
		var.vt = VT_UNKNOWN;
		var.punkVal = (IUnknown*)pFrame;

		VARIANT_BOOL vbRes;
		pTransform->SetSource(0, var);
		pTransform->SetXml(L"<ImageFile-SaveAsJpeg destinationpath=\"c:\\out.jpg\" format=\"888\" quality=\"100\"></ImageFile-SaveAsJpeg>", &vbRes);
		pTransform->Transform(&vbRes);
	}

private:
	CString            m_transform_string;

	MediaCore::IAVSUncompressedVideoFrame* m_pThumbnailImage;
	ImageStudio3::IImageTransforms* m_pTransforms;

	Bitmap* m_pBitmapThumbnail;
};

#endif