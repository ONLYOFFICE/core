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
#include <string>

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
	HRESULT hResult;

	hResult = m_imageraw_core._open(fileName);

	if (hResult)
		return CImageRawATLError::Error(IMAGERAW_OPENFILE_ERROR,__uuidof(IImageRaw3));
	
	if (m_OpenFileWithoutLoadData == FALSE) //  OpenFile
	{
		hResult = m_imageraw_core._load(&m_OptionsSimple);
		
		if (hResult)
			return CImageRawATLError::Error(IMAGERAW_OPENFILE_ERROR,__uuidof(IImageRaw3));
	}
	return S_OK;
}

STDMETHODIMP CImageRaw3::ApplyXML(BSTR bsSettingXML,LONG lMode)
{
	return m_OptionsSimple.OpenXML(bsSettingXML);
}

STDMETHODIMP CImageRaw3::GetXML(BSTR * bsSettingXML, LONG lMode)
{
    _ASSERTE(NULL != bsSettingXML);
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
    {
        _ASSERTE(SUCCEEDED(hr) && "Faield CoCreateInstance(CLSID_CAVSUncompressedVideoFrame)");
		return hr;
    }

	int nBufferSize = 3*nWidth*nHeight;
	pMediaData->put_ColorSpace(CSP_BGR);
	pMediaData->put_Width(nWidth);
	pMediaData->put_Height(nHeight);
	pMediaData->put_AspectRatioX(nWidth);
	pMediaData->put_AspectRatioY(nHeight);
	pMediaData->put_Interlaced(VARIANT_FALSE);
	pMediaData->put_Stride(0, 3*nWidth);
	if FAILED(hr = pMediaData->AllocateBuffer(nBufferSize))
    {
        _ASSERTE(SUCCEEDED(hr) && "Failed IAVSMediaData::AllocateBuffer");
		return hr;
    }
		
	long nCreatedBufferSize = 0;
	BYTE * pBufferPtr = NULL;
	pMediaData->get_Buffer(&pBufferPtr);
	pMediaData->get_BufferSize(&nCreatedBufferSize);
    _ASSERTE(nCreatedBufferSize >= nBufferSize);
    if (nCreatedBufferSize < nBufferSize)
		return E_IMAGERAW(IMAGERAW_MEMORY_ERROR);

	//pMediaData->put_Plane(0, pBufferPtr);
	

	memcpy_s(pBufferPtr,nCreatedBufferSize,ppm,3*nWidth*nHeight);
	
	hr = pMediaData.CopyTo((MediaCore::IAVSUncompressedVideoFrame**)piUncFrame);
    _ASSERTE(SUCCEEDED(hr));

	return hr;
}

HRESULT CImageRaw3::LoadPPMThumbnail(BYTE * thumb_data, size_t thumb_size, IUnknown** piUnkFrame)
{
    _ASSERTE(NULL != piUnkFrame);
    _ASSERTE(NULL != thumb_data);
    if (NULL == piUnkFrame || NULL == thumb_data)
        return E_POINTER;

    if (0 == thumb_size)
        return E_IMAGERAW(IMAGERAW_UNSUPPORTEDTHUMBFORMAT_ERROR);
        
    unsigned int width, height, colors;
    _ASSERTE(thumb_size >= 12);
    if (thumb_size < 12)
        return E_IMAGERAW(IMAGERAW_UNSUPPORTEDTHUMBFORMAT_ERROR);

    int cnt = sscanf_s((const char*)thumb_data, "P6\n%d %d\n%d\n", &width, &height, &colors);
    _ASSERTE(cnt == 3);    
    if (cnt != 3)
        return E_IMAGERAW(IMAGERAW_UNSUPPORTEDTHUMBFORMAT_ERROR);
    
    char buffer[255];
    sprintf_s(buffer, "P6\n%d %d\n%d\n", width, height, colors);
    size_t headerLen = strlen(buffer);
    
    return LoadPPMThumbnailWithoutHeader(width, height, colors, thumb_data + headerLen, thumb_size - headerLen, piUnkFrame, m_imageraw_core._get_dcraw()->flip);

//	FILE * f = fopen("c:\\ppm_thumb.ppm","wb");
//	fwrite(thumb_data, thumb_size, 1, f);
//	fclose(f);
	
}

HRESULT CImageRaw3::LoadPPMThumbnailWithoutHeader(size_t _Width, size_t _Height, unsigned int _Colors, BYTE * thumb_data, size_t thumb_size, IUnknown** piUnkFrame, int _Flip)
{
    HRESULT hr;
    _ASSERTE(255 == _Colors);
    if (_Colors != 255)
        return E_IMAGERAW(IMAGERAW_UNSUPPORTEDTHUMBFORMAT_ERROR);

    if (_Width * _Height * 3 > thumb_size)
        return E_IMAGERAW(IMAGERAW_UNSUPPORTEDTHUMBFORMAT_ERROR);

    CComPtr<MediaCore::IAVSUncompressedVideoFrame> pMediaData;
	if FAILED(hr = pMediaData.CoCreateInstance(MediaCore::CLSID_CAVSUncompressedVideoFrame))
		return hr;

	int nBufferSize = 4*_Width*_Height;
	pMediaData->put_ColorSpace(CSP_BGRA);
    pMediaData->put_Width( ((_Flip == 5) || (_Flip == 6)) ? _Height : _Width);
	pMediaData->put_Height(((_Flip == 5) || (_Flip == 6)) ? _Width : _Height);
	pMediaData->put_AspectRatioX(((_Flip == 5) || (_Flip == 6)) ? _Height : _Width);
	pMediaData->put_AspectRatioY(((_Flip == 5) || (_Flip == 6)) ? _Width : _Height);
	pMediaData->put_Interlaced(VARIANT_FALSE);
	pMediaData->put_Stride(0, 4*_Width);
	if FAILED(hr = pMediaData->AllocateBuffer(nBufferSize))
		return hr;
		
	long nCreatedBufferSize = 0;
	BYTE * pBufferPtr = NULL;
	pMediaData->get_Buffer(&pBufferPtr);
	pMediaData->get_BufferSize(&nCreatedBufferSize);
	if (nCreatedBufferSize != nBufferSize)
		return E_IMAGERAW(IMAGERAW_MEMORY_ERROR);

	pMediaData->put_Plane(0, pBufferPtr);

    size_t nWidth = _Width;
    size_t nHeight = _Height;

    for (size_t x = 0; x < nWidth; ++x)
    {
        for (size_t y = 0; y < nHeight; ++y)
        {
            unsigned int offset1;
            if (6 == _Flip)
                offset1 = x * nHeight * 4 + (nHeight - y - 1) * 4;
            else if (5 == _Flip)
                offset1 = (nWidth - x - 1) * nHeight * 4 + y * 4;
            else
                offset1 = y * nWidth * 4 + x * 4;

            unsigned int offset2 = y * nWidth * 3 + x * 3;

            pBufferPtr[offset1 + 0] = thumb_data[offset2 + 2];
            pBufferPtr[offset1 + 1] = thumb_data[offset2 + 1];
            pBufferPtr[offset1 + 2] = thumb_data[offset2 + 0];
            pBufferPtr[offset1 + 3] = 255;
        }
    }

    return pMediaData.CopyTo((MediaCore::IAVSUncompressedVideoFrame**)piUnkFrame);
}

HRESULT CImageRaw3::LoadGdiplusThumbnail(BYTE * thumb_data, size_t thumb_size, IUnknown** piUnkFrame)
{
	_HR;
	INT flip,tmpI;
	Bitmap * bitmap;
	CComQIPtr<MediaCore::IAVSUncompressedVideoFrame> pMediaData;
	UINT nWidth,nHeight;
	Gdiplus::Status gdi_state;
	void * pMem = NULL;

	if (!piUnkFrame)
		return E_POINTER;

	*piUnkFrame = NULL;

	CAutoMemPtr_<BYTE> auto_thumb_data(thumb_data);

    _ASSERTE(!!thumb_data);
    _ASSERTE(!!thumb_size);
    // thumbnail не загружен
	if (!thumb_data || !thumb_size)
    {
		return E_IMAGERAW(IMAGERAW_LOAD_THUMBNAIL_ERROR);		
    }

	// создаем stream, чтобы потом прочитать из него изображение через Gdi+
	// выделяем под stream глобальную память
	HGLOBAL hgMem = ::GlobalAlloc(GHND,thumb_size);
    _ASSERTE(!!hgMem);
	if (!hgMem)
		return E_OUTOFMEMORY;

	// для автоматического освобождения
	CAutoHGLOBALFree auto_hgMem(hgMem);
	
	// заполняем память
	if (!(pMem = ::GlobalLock(hgMem)))
    {
        _ASSERTE(!!pMem);
		return E_IMAGERAW(IMAGERAW_MEMORY_ERROR);
    }

	if (memcpy_s(pMem,thumb_size,auto_thumb_data,thumb_size))
    {
        _ASSERTE(false && "Failed memcpy_s");
		return E_IMAGERAW(IMAGERAW_MEMORY_ERROR);
    }

	if (::GlobalUnlock(hgMem))
    {
        _ASSERTE(false && "Failed GlobalUnlock");
		return E_IMAGERAW(IMAGERAW_MEMORY_ERROR);
    }
	
	CComPtr<IStream> spStream;
	// создаем stream, который автоматически осовободит память hgMem на Release
	if FAILED(hr = ::CreateStreamOnHGlobal(hgMem,TRUE,&spStream))
    {
        _ASSERTE(false && "Failed CreateStreamOnHGlobal");
		return E_IMAGERAW(IMAGERAW_UNEXPECTED_ERROR);
    }

	// больше не требуется сопровождать память, так как теперь сопровождается объектом stream
	auto_hgMem.Detach();

	// создаем битмап и пытаемся прочитать в него из стрима
	bitmap = new Bitmap(spStream);
	std::auto_ptr<Bitmap> _bitmap_ptr(bitmap);

	if (!bitmap)
		return E_IMAGERAW(IMAGERAW_UNEXPECTED_ERROR);
	
	if ((gdi_state = bitmap->GetLastStatus() )!= Gdiplus::Status::Ok)
    {
        _ASSERTE(Gdiplus::Status::Ok == gdi_state);
		return E_IMAGERAW(IMAGERAW_DECODE_THUMBNAIL_ERROR);
    }
	
	// создаем экземпляр медиадаты
	if FAILED(hr = pMediaData.CoCreateInstance(MediaCore::CLSID_CAVSUncompressedVideoFrame))
    {
        _ASSERTE(SUCCEEDED(hr) && "Failed CoCreateInstance(CLSID_CAVSUncompressedVideoFrame)");
		return hr;
    }

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
    {
        _ASSERTE(SUCCEEDED(hr) && "Failed IAVSMediaData::AllocateBuffer");
		return hr;
    }
		
	long nCreatedBufferSize = 0;
	BYTE * pBufferPtr = NULL;
	pMediaData->get_Buffer(&pBufferPtr);
	pMediaData->get_BufferSize(&nCreatedBufferSize);
	if (nCreatedBufferSize != nBufferSize)
    {
        _ASSERTE(nCreatedBufferSize == nBufferSize);
		return E_IMAGERAW(IMAGERAW_MEMORY_ERROR);
    }

	pMediaData->put_Plane(0, pBufferPtr);
	
	if FAILED(hr = ImageRaw3::ImageToMemory(bitmap,pBufferPtr))
    {
        _ASSERTE(SUCCEEDED(hr) && "Failed ImageRaw3::ImageToMemory");
		return	hr;
    }

	// копируем изображение из bitmap-а
	return pMediaData.CopyTo((MediaCore::IAVSUncompressedVideoFrame**)piUnkFrame);

}

STDMETHODIMP CImageRaw3::GetThumbnailImage(IUnknown** piUnkFrame)
{
	_HR;
	BYTE * thumb_data;
	size_t thumb_size;
	_ASSERTE(NULL != piUnkFrame);
	if (!piUnkFrame)
		return E_POINTER;

	*piUnkFrame = NULL;
	
	_CHECK_RET_IR3(m_imageraw_core._get_thumbnail((void**)&thumb_data, &thumb_size));
	
	bool isPPMThumbnail = false;
	_CHECK_RET_IR3(m_imageraw_core._is_ppm_thumbnail(&isPPMThumbnail));

	if (!isPPMThumbnail)
		return LoadGdiplusThumbnail(thumb_data, thumb_size, piUnkFrame);
	else
		return LoadPPMThumbnail(thumb_data, thumb_size, piUnkFrame);

}

STDMETHODIMP CImageRaw3::RenderImage(IUnknown** piUncFrame)
{
    _ASSERTE(NULL != piUncFrame);
	piVideoFrame->AddRef();
	*piUncFrame = piVideoFrame;
	return S_OK;
}

STDMETHODIMP CImageRaw3::SetAdditionalParam(BSTR ParamName, VARIANT ParamValue)
{
	CComBSTR strParamName(ParamName);

	if (strParamName == L"setOpenFileWithoutLoadData")// set mode OpenFile / IsRawFile
	{
		if (ParamValue.vt == VT_BOOL)
			m_OpenFileWithoutLoadData = ParamValue.bVal;

	}	
	return S_OK;
}

STDMETHODIMP CImageRaw3::GetAdditionalParam(BSTR ParamName, VARIANT* ParamValue)
{

	return S_OK;
}


STDMETHODIMP CImageRaw3::CloseFile(void)
{
	HRESULT hr;
	_CHECK_RET_IR3(m_imageraw_core._close());
	return S_OK;
}


STDMETHODIMP CImageRaw3::LoadThumbnail(BSTR fileName, IUnknown** piUncFrame)
{
	_HR;
	_ASSERTE(NULL != piUncFrame);
	if (!piUncFrame)
		return E_POINTER;

	_CHECK_RET_IR3(m_imageraw_core._open(fileName));
	hr = GetThumbnailImage(piUncFrame);
	_CHECK_RET_IR3(m_imageraw_core._close());
	return hr;
}
