#include "stdafx.h"
#include "ImageRaw3.h"
#include "ImageRawATLError.h"

#include "ImageRawCommon.h"

#define _RET_ERROR_IR3(HR) return CImageRawATLError::Error(hr = (HR),__uuidof(IImageRaw3))

#define _RET_IR3(HR) 	if (FAILED(hr = (HR))) \
							_RET_ERROR_IR3(hr); \
						else \
							return hr;

#define _CHECK_RET_IR3(HR) 	do {_CHECK_HR(hr = (HR)); if (FAILED(hr)){ _RET_IR3(hr);}} while(0);


HRESULT CImageRaw3::GetMetadata(BSTR fileName)
{
	HRESULT hr;
	_CHECK_RET_IR3(m_imageraw_core._open(fileName));

	if (!(m_imageraw_core._get_dcraw()))
		return E_FAIL;

	
	if (m_imageraw_core._get_dcraw()->_ifd_entrys.size() <= 0)
		  return S_FALSE;
	else
		return S_OK;
}
/*
HRESULT CImageRaw3::GetIFD(BSTR bstrReserved, DWORD dwReserved, DWORD * dwOffset)
{
	if (!dwOffset)
		return E_POINTER;

	if (!(m_imageraw_core._get_dcraw()))
		return E_FAIL;

	if (!(m_imageraw_core._get_dcraw()->_has_exif))
		return E_FAIL;
	
	if (0 == dwReserved)
		*dwOffset = m_imageraw_core._get_dcraw()->_exif_ifd_offset;
	else if (1 == dwReserved)
		*dwOffset = m_imageraw_core._get_dcraw()->_exif_base_offset;
	else
		return E_INVALIDARG;
	
	return S_OK;
}*/

HRESULT CImageRaw3::IFDCount(DWORD dwReserved, DWORD * dwCount)
{
	if (!dwCount)
		return E_POINTER;
	
	*dwCount = 0;

	if (!(m_imageraw_core._get_dcraw()))
		return E_FAIL;

	*dwCount = m_imageraw_core._get_dcraw()->_ifd_entrys.size();
	return S_OK;
}

HRESULT CImageRaw3::IFDTag(DWORD dwIndex, USHORT * dwTag)
{
	if (!dwTag)
		return E_POINTER;
	*dwTag = -1;

	if (!(m_imageraw_core._get_dcraw()))
		return E_FAIL;
 
	size_t size_ = m_imageraw_core._get_dcraw()->_ifd_entrys.size();
	
	if (dwIndex >= size_)
		return E_INVALIDARG;

	*dwTag = m_imageraw_core._get_dcraw()->_ifd_entrys[dwIndex]._ifd_tag;
	return S_OK;
}

HRESULT CImageRaw3::IFDOffset(DWORD dwIndex, DWORD * dwOffset)
{
	if (!dwOffset)
		return E_POINTER;
	*dwOffset = 0;
	if (!(m_imageraw_core._get_dcraw()))
		return E_FAIL;

	size_t size_ = m_imageraw_core._get_dcraw()->_ifd_entrys.size();

	if (dwIndex >= size_)
		return E_INVALIDARG;

	*dwOffset = m_imageraw_core._get_dcraw()->_ifd_entrys[dwIndex]._ifd_ifd_offset;
	return S_OK;

}

HRESULT CImageRaw3::BaseOffset(DWORD dwIndex, DWORD * dwOffset)
{
	if (!dwOffset)
		return E_POINTER;
	*dwOffset = 0;
	if (!(m_imageraw_core._get_dcraw()))
		return E_FAIL;

	size_t size_ = m_imageraw_core._get_dcraw()->_ifd_entrys.size();

	if (dwIndex >= size_)
		return E_INVALIDARG;

	*dwOffset = m_imageraw_core._get_dcraw()->_ifd_entrys[dwIndex]._ifd_base_offset;
	return S_OK;
}



HRESULT CImageRaw3::GetFlags(DWORD dwReserved, DWORD *dwFlags)
{
	if (!dwFlags)
		return E_POINTER;

	*dwFlags = 0x4949;
	if (!(m_imageraw_core._get_dcraw()))
		return E_FAIL;
	
	*dwFlags = m_imageraw_core._get_dcraw()->order;
	return S_OK;	
}