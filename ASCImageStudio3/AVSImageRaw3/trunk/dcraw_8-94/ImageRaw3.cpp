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

#define _RET_ERROR_IR3(HR) return CImageRawATLError::Error(hr = (HR),__uuidof(IImageRaw3))

#define _RET_IR3(HR) 	if (FAILED(hr = (HR))) \
							_RET_ERROR_IR3(hr); \
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
	_CHECK_RET_IR3(m_imageraw_core._load(&m_OptionsSimple));
	return S_OK;
}

STDMETHODIMP CImageRaw3::ApplyXML(BSTR bsSettingXML,LONG lMode)
{
	return m_OptionsSimple.OpenXML(bsSettingXML);
}

STDMETHODIMP CImageRaw3::GetXML(BSTR * bsSettingXML, LONG lMode)
{
	if (!bsSettingXML)
		return E_POINTER;

	return m_OptionsSimple.CopyXMLTo(bsSettingXML);
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

	/*m_OptionsSimple.UseTemperature = true;
	m_OptionsSimple.Temperature.temperature = 5000;
	m_OptionsSimple.Temperature.green = 1.0;*/

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
