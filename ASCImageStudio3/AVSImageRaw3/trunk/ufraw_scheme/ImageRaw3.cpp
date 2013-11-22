/*! \file	ImageRaw3.cpp
 *	\brief	Implementation of CImageRaw3
 *
 *	\author	Nikolay.Pogorskiy@avsmedia.net
 */


#include "stdafx.h"
#include "gdiplus.h"
#include "ImageRaw3.h"
#include "ImageRawATLError.h"
#include "..\..\..\..\AVSVideoStudio3\Common\ImageStudioUtils.h"
#include "ImageRawCommon.h"

#define _RET_ERRROR_IR3(HR) return CImageRawATLError::Error(hr = (HR),__uuidof(IImageRaw3))

#define _RET_IR3(HR) 	if (FAILED(hr = (HR))) \
							_RET_ERRROR_IR3(hr); \
						else \
							return hr;

#define _CHECK_RET_IR3(HR) 	do {_CHECK_HR(hr = (HR)); if (FAILED(hr)){ _RET_IR3(hr);}} while(0);



void CImageRaw3::_ExtractImage()
{
		
}

STDMETHODIMP CImageRaw3::OpenFile(BSTR fileName)
{
	_HR;
	_CHECK_RET_IR3(m_imageraw_core._open(fileName));
	_CHECK_RET_IR3(m_imageraw_core._load());
	return S_OK;

	//_HR;
	//_CHECK_RET_IR3(ufraw_data._open(fileName));

	//return S_OK;

//	conf_data raw_config;
//	conf_init(&raw_config);
//	_CHECK_RET_IR3(ufraw_data._config(&raw_config,NULL,NULL));
//
//
//	_CHECK_RET_IR3(ufraw_data._load_raw());
//
//	_CHECK_RET_IR3(ufraw_data._convert_image());
//
//	ufraw_data._convert_image_init_phase();
//
//	ufraw_image_data * im1 = ufraw_data._convert_image_develop();
//
///*	FILE *fil = fopen("E:/Work/AVS/ActiveX/AVS/Sources/AVSImageStudio3/AVSImageRaw3/trunk/workingcopy/01.ppm","wb");
//
//	ufraw_data.GetUFRawData()->raw->GetDCRawData()->GetDCRaw()->write_ppm_tiff(fil);
//	fclose(fil);*/
//	
//	image_data * im = &(ufraw_data.GetUFRawData()->image);
//	//ufraw_image_data * im1 = &(ufraw_data.GetUFRawData()->Images[0]);
//	/*ufraw_image_data * im2 = &(ufraw_data.GetUFRawData()->Images[1]);
//	ufraw_image_data * im3 = &(ufraw_data.GetUFRawData()->Images[2]);
//	ufraw_image_data * im4 = &(ufraw_data.GetUFRawData()->Images[3]);*/
//
//	
//	/*if (FAILED(hr = CoCreateInstance(MediaCore::CLSID_CAVSUncompressedVideoFrame,
//		NULL,
//		CLSCTX_INPROC,
//		MediaCore::IID_IAVSUncompressedVideoFrame,
//		(void**)&piVideoFrame)))
//		return hr;*/
//
//	SAFEARRAYBOUND rgsaBound[3];
//	rgsaBound[1].cElements = im1->width;
//	rgsaBound[1].lLbound = 0;
//	rgsaBound[2].cElements = im1->height;
//	rgsaBound[2].lLbound = 0;
//	rgsaBound[0].cElements = 3;
//	rgsaBound[0].lLbound = 0;
//	
//	SAFEARRAY * sa = ::SafeArrayCreate(VT_UI1,3,rgsaBound);
//	
//	if (!sa)
//		return E_FAIL;
//
//	
//	for (int y = 0;y<im1->height;y++)
//	{
//		for (int x = 0;x<im1->width;x++)	
//		{
//			for (int c = 0;c<3;c++)
//			{
//			/*	((BYTE*)(sa->pvData))[4*im1->width*y + 4*x + c] 
//				= ((unsigned long)255*((unsigned short *)(im1->buffer))[4*im1->width*y + 4*x + c])/65535;// [8*im1->width + 2*(x+c)]);*/
//
//				((BYTE*)(sa->pvData))[3*im1->width*y + 3*x + (c)] 
//				= /*(255*(unsigned long)*/
//					/*(*/ *(((unsigned char *)(im1->buffer + im1->rowstride*y + 3*x + c))); /*&0xFF00 )>>8;*/
//				 /*	)/10000;*/
//
//			}
//		}
//	}
//
//	// compute safearray's image size
//	int nWidth = sa->rgsabound[1].cElements;
//	int nHeight = sa->rgsabound[0].cElements;
//
//
//	MediaCore::IAVSUncompressedVideoFrame* pMediaData = NULL;
//
//	CoCreateInstance(MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)(&pMediaData));
//
//	pMediaData->put_ColorSpace(CSP_BGR);
//
//	// specify settings
//	pMediaData->put_Width(nWidth);
//	pMediaData->put_Height(nHeight);
//	pMediaData->put_AspectRatioX(nWidth);
//	pMediaData->put_AspectRatioY(nHeight);
//	pMediaData->put_Interlaced(VARIANT_FALSE);
//	pMediaData->put_Stride(0, 3*nWidth);
//	pMediaData->AllocateBuffer(3*nWidth*nHeight);
//	
//	// allocate necesasry buffer
//	BYTE* pBufferPtr = 0;
//	long nCreatedBufferSize = 0;
//	pMediaData->get_Buffer(&pBufferPtr);
//	pMediaData->get_BufferSize(&nCreatedBufferSize);
//	pMediaData->put_Plane(0, pBufferPtr);
//
//	
//	memcpy(pBufferPtr, sa->pvData, nCreatedBufferSize);
//
////	ImageStudioUtils::SafeArrayToMediaData(sa,(IUnknown**)&piVideoFrame,TRUE);
//
//	piVideoFrame = pMediaData;
//
//	_RET_IR3(hr);
}

STDMETHODIMP CImageRaw3::ApplyXML(BSTR bsSettingXML,LONG lMode)
{
	return S_OK;
}

STDMETHODIMP CImageRaw3::GetXML(BSTR * bsSettingXML, LONG lMode)
{
	return S_OK;
}


STDMETHODIMP CImageRaw3::GetMetaData(BSTR * bsMetaDataXML)
{
	return S_OK;
}

STDMETHODIMP CImageRaw3::GetPreviewImage(IUnknown** piUncFrame)
{
	_HR;
	unsigned char * ppm = NULL;
	CComQIPtr<MediaCore::IAVSUncompressedVideoFrame> pMediaData;
	UINT nWidth,nHeight,nColors;	

	_CHECK_RET_IR3(m_imageraw_core._get_preview(&m_OptionsSimple,(void **)&ppm,&nWidth,&nHeight,&nColors));

	CAutoMemPtr_<unsigned char> auto_data(ppm);

	// создаем экземпляр медиадаты
	if FAILED(hr = pMediaData.CoCreateInstance(MediaCore::CLSID_CAVSUncompressedVideoFrame))
		return hr;

	int nBufferSize = 3*nWidth*nHeight;
	pMediaData->put_ColorSpace(CSP_BGR);
	pMediaData->put_Width(nWidth);
	pMediaData->put_Height(nHeight);
	pMediaData->put_AspectRatioX(nWidth);
	pMediaData->put_AspectRatioY(nHeight);
	pMediaData->put_Interlaced(VARIANT_FALSE);
	pMediaData->put_Stride(0, 3*nWidth);
	if FAILED(hr = pMediaData->AllocateBuffer(nBufferSize))
		return hr;
		
	long nCreatedBufferSize = 0;
	BYTE * pBufferPtr = NULL;
	pMediaData->get_Buffer(&pBufferPtr);
	pMediaData->get_BufferSize(&nCreatedBufferSize);
	if (nCreatedBufferSize != nBufferSize)
		return E_IMAGERAW(IMAGERAW_MEMORY_ERROR);

	//pMediaData->put_Plane(0, pBufferPtr);
	

	memcpy_s(pBufferPtr,nCreatedBufferSize,ppm,3*nWidth*nHeight);
	
	hr = pMediaData.CopyTo((MediaCore::IAVSUncompressedVideoFrame**)piUncFrame);

	return S_OK;
}

STDMETHODIMP CImageRaw3::GetThumbnailImage(IUnknown** piUnkFrame)
{
	_HR;
	INT flip,tmpI;
	BYTE * thumb_data;
	size_t thumb_size;
	Gdiplus::Status gdi_state;
	void * pMem = NULL;
	Bitmap * bitmap;
	CComQIPtr<MediaCore::IAVSUncompressedVideoFrame> pMediaData;
	UINT nWidth,nHeight;

	if (!piUnkFrame)
		return E_POINTER;

	*piUnkFrame = NULL;

	//if (!ufraw_data.GetUFRawData())
	//	return E_IMAGERAW(IMAGERAW_FILE_NOT_OPENED_ERROR);

	//if (!ufraw_data.GetUFRawData()->raw)
	//	return E_IMAGERAW(IMAGERAW_FILE_NOT_OPENED_ERROR);

	//if (!ufraw_data.GetUFRawData()->raw->GetDCRawData())
	//	return E_IMAGERAW(IMAGERAW_FILE_NOT_OPENED_ERROR);
	
	_CHECK_RET_IR3(m_imageraw_core._get_thumbnail((void**)&thumb_data,&thumb_size));

	CAutoMemPtr_<BYTE> auto_thumb_data(thumb_data);

	// thumbnail не загружен
	if (!thumb_data || !thumb_size)
		return E_IMAGERAW(IMAGERAW_LOAD_THUMBNAIL_ERROR);		

	// создаем stream, чтобы потом прочитать из него изображение через Gdi+
	// выделяем под stream глобальную память
	HGLOBAL hgMem = ::GlobalAlloc(GHND,thumb_size);
	if (!hgMem)
		return E_OUTOFMEMORY;

	// для автоматического освобождения
	CAutoHGLOBALFree auto_hgMem(hgMem);
	
	// заполняем память
	if (!(pMem = ::GlobalLock(hgMem)))
		return E_IMAGERAW(IMAGERAW_MEMORY_ERROR);

	if (memcpy_s(pMem,thumb_size,auto_thumb_data,thumb_size))
		return E_IMAGERAW(IMAGERAW_MEMORY_ERROR);

	if (::GlobalUnlock(hgMem))
		return E_IMAGERAW(IMAGERAW_MEMORY_ERROR);
	
	CComPtr<IStream> spStream;
	// создаем stream, который автоматически осовободит память hgMem на Release
	if FAILED(hr = ::CreateStreamOnHGlobal(hgMem,TRUE,&spStream))
		return E_IMAGERAW(IMAGERAW_UNEXPECTED_ERROR);

	// больше не требуется сопровождать память, так как теперь сопровождается объектом stream
	auto_hgMem.Detach();

	// создаем битмап и пытаемся прочитать в него из стрима
	bitmap = new Bitmap(spStream);

	if (!bitmap)
		return E_IMAGERAW(IMAGERAW_UNEXPECTED_ERROR);
	
	if ((gdi_state = bitmap->GetLastStatus() )!= Gdiplus::Status::Ok)
		return E_IMAGERAW(IMAGERAW_DECODE_THUMBNAIL_ERROR);
	
	// создаем экземпляр медиадаты
	if FAILED(hr = pMediaData.CoCreateInstance(MediaCore::CLSID_CAVSUncompressedVideoFrame))
		return hr;

	// заполняем поля медиадаты
	nWidth = bitmap->GetWidth();
	nHeight = bitmap->GetHeight();

	// учитываем ориентацию камеры
	// todo: возможно, некоторые камеры записывают thumbnail в уже скорректированой ориентации
	switch(flip = m_imageraw_core._get_dcraw()->flip)
	{
	case 1:/* Flip horizontal */
		bitmap->RotateFlip(RotateNoneFlipX);
		break;
	case 2: /* Flip vertical */ 
		bitmap->RotateFlip(RotateNoneFlipY);
		break;
	case 3: /* Rotate 180 */ 
		bitmap->RotateFlip(Rotate180FlipNone);
		break;

	// \todo	Flip over diagonal "\" 
	case 4:   
		break;

	case 5: /* Rotate 270 */ 
		tmpI = nWidth;	nWidth = nHeight; nHeight = tmpI;
		bitmap->RotateFlip(Rotate270FlipNone);
		break;

	case 6: /* Rotate 90 */ 
		tmpI = nWidth;	nWidth = nHeight; nHeight = tmpI;
		bitmap->RotateFlip(Rotate90FlipNone);
		break;

	// \todo	Flip over diagonal "/"  
	case 7:
		break;
	default:
		break;
	}

	int nBufferSize = 4*nWidth*nHeight;
	pMediaData->put_ColorSpace(CSP_BGRA | CSP_VFLIP);
	pMediaData->put_Width(nWidth);
	pMediaData->put_Height(nHeight);
	pMediaData->put_AspectRatioX(nWidth);
	pMediaData->put_AspectRatioY(nHeight);
	pMediaData->put_Interlaced(VARIANT_FALSE);
	pMediaData->put_Stride(0, 4*nWidth);
	if FAILED(hr = pMediaData->AllocateBuffer(nBufferSize))
		return hr;
		
	long nCreatedBufferSize = 0;
	BYTE * pBufferPtr = NULL;
	pMediaData->get_Buffer(&pBufferPtr);
	pMediaData->get_BufferSize(&nCreatedBufferSize);
	if (nCreatedBufferSize != nBufferSize)
		return E_IMAGERAW(IMAGERAW_MEMORY_ERROR);

	pMediaData->put_Plane(0, pBufferPtr);
	
	if FAILED(hr = ImageRaw3::ImageToMemory(bitmap,pBufferPtr))
		return	hr;

	// копируем изображение из bitmap-а
	return pMediaData.CopyTo((MediaCore::IAVSUncompressedVideoFrame**)piUnkFrame);
}

STDMETHODIMP CImageRaw3::RenderImage(IUnknown** piUncFrame)
{
	piVideoFrame->AddRef();
	*piUncFrame = piVideoFrame;
	return S_OK;
}

STDMETHODIMP CImageRaw3::SetAdditionalParam(BSTR ParamName, VARIANT ParamValue)
{
	return S_OK;
}

STDMETHODIMP CImageRaw3::GetAdditionalParam(BSTR ParamName, VARIANT* ParamValue)
{
	return S_OK;
}


STDMETHODIMP CImageRaw3::CloseFile(void)
{
	return S_OK;
}
