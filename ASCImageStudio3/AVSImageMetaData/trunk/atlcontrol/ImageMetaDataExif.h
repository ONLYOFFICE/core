/*!	\file	ImageMetaDataExif.h
*	\breif	Declaration of the CImageMetaDataExif
*/

#pragma once
#include "resource.h"       // main symbols
#include "IImageMetaData.h"
#include "metadatatypes.h"

#include "..\metadataparsers\exifparser.h"
#include <string>
#include "..\XML\MetaDataTransform.h"
#include "MetaDataInterface.h"

/*!	\class	CImageMetaDataExif
*/
[
	coclass,
	default(IImageMetaDataExif),
	threading(apartment),
	support_error_info("IImageMetaDataExif"),
	vi_progid("AVSImageMetaData.ImageMetaDataExif"),
	progid("AVSImageMetaData.ImageMetaDataExif.1"),
	version(1.0),
	uuid("B4F8D2A2-ACA2-430B-9C40-5BCC2141E055"),
	helpstring("ImageMetaDataExif Class")
]
class ATL_NO_VTABLE CImageMetaDataExif :
	public IImageMetaDataExif,
	public AVS::Image::Details::ImageMetaDataCommonImpl
{
	friend class CImageMetaDataParser;
public:
	CImageMetaDataExif():m_dwTransformFlag(AVS::Image::XML::MetaDataXMLTransform::TRANSFORM_RAWMAKERNOTETAGS)
	{

	}

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	STDMETHOD(GetValue)(USHORT TagId, BSTR * bstrValue)
	{
		HRESULT hr;
		if (!bstrValue)
			return E_POINTER;
		*bstrValue = NULL;
		BSTR _bstrInputXML = NULL;
		if FAILED(hr = GetXML(0, &_bstrInputXML))
			return hr;
		BSTR _bstrTagValue = NULL;
		unsigned short _nTagType = 0;
		unsigned int _nTagCount = 0;
		if (!AVS::Image::XML::MetaDataXMLTransform::GetInfoByTagId(TagId, _bstrInputXML, &_bstrTagValue, _nTagCount, _nTagType))
			return E_FAIL;
		*bstrValue = _bstrTagValue;
		return S_OK;
	}

	STDMETHOD(GetType)(USHORT TagId, USHORT * nType)
	{
		HRESULT hr;
		if (!nType)
			return E_POINTER;
		nType = 0;
		BSTR _bstrInputXML = NULL;
		if FAILED(hr = GetXML(0, &_bstrInputXML))
			return hr;
		BSTR _bstrTagValue = NULL;
		unsigned short _nTagType = 0;
		unsigned int _nTagCount = 0;
		if (!AVS::Image::XML::MetaDataXMLTransform::GetInfoByTagId(TagId, _bstrInputXML, &_bstrTagValue, _nTagCount, _nTagType))
			return E_FAIL;
		*nType = _nTagType;
		return S_OK;
	}

	STDMETHOD(GetCount)(USHORT TagId, UINT * nCount)
	{
		HRESULT hr;
		if (!nCount)
			return E_POINTER;
		nCount = 0;
		BSTR _bstrInputXML = NULL;
		if FAILED(hr = GetXML(0, &_bstrInputXML))
			return hr;
		BSTR _bstrTagValue = NULL;
		unsigned short _nTagType = 0;
		unsigned int _nTagCount = 0;
		if (!AVS::Image::XML::MetaDataXMLTransform::GetInfoByTagId(TagId, _bstrInputXML, &_bstrTagValue, _nTagCount, _nTagType))
			return E_FAIL;
		*nCount = _nTagCount;
		return S_OK;
	}

private:

	/*!
	\brief	«агружаем XML с описанием Exif-тегов из ресурсов dll (ресурс IDR_XML_EXIF_DESC)
	*/
	bool LoadExifXML(std::wstring & str)
	{
		str = L"";
		HINSTANCE hResInstance = ATL::_pModule->GetResourceInstance();
		if (!hResInstance)
			return false;
		HRSRC hRes = FindResourceW((HMODULE)hResInstance, MAKEINTRESOURCEW(IDR_XML_EXIF_DESC), MAKEINTRESOURCEW(RT_HTML));
		if (!hRes)
			return false;
		DWORD sizeofXML = SizeofResource((HMODULE)hResInstance, hRes); 
		HGLOBAL hData = LoadResource((HMODULE) hResInstance, hRes);
		if (!hData)
			return false;
		LPVOID lpData = LockResource(hData);
		if (!lpData)
			return FALSE;
		char * resourceXML = new char[sizeofXML+1];
		memset(resourceXML, 0, sizeofXML+1);
		memcpy_s(resourceXML, sizeofXML+1, lpData, sizeofXML);
		std::wstring resultXML(AVS::Utils::__StoW((char*)resourceXML));
		free(resourceXML);
		str = resultXML;
		return true;
	}

	// IImageMetaDataCommon Methods
public:
	STDMETHOD(get_MetaDataType)(DWORD * dwMode)
	{
		if (NULL == dwMode)
			return E_POINTER;
		*dwMode = AVS::Image::MetaDataTypes::Exif;
		return S_OK;
	}

	STDMETHOD(GetCommonXML)(DWORD dwMode, BSTR * bstrMetaDataXML)
	{
		HRESULT hr;
		if (NULL == bstrMetaDataXML)
			return E_POINTER;
		*bstrMetaDataXML = NULL;
		BSTR bsExifXml = NULL;
		if FAILED(hr = GetXML(0, &bsExifXml))
			return hr;
		std::wstring _ExifDesc;
		bool bResult = LoadExifXML(_ExifDesc);
		BSTR bsResultXml = NULL;
		if (!AVS::Image::XML::MetaDataXMLTransform::TransformExifToCommon(bsExifXml, CComBSTR(_ExifDesc.c_str()), &bsResultXml, m_dwTransformFlag))
			return E_FAIL;
		*bstrMetaDataXML = bsResultXml;
		return S_OK;
	}

	STDMETHOD(GetXML)(DWORD dwMode, BSTR * bstrMetaDataXML)
	{
		if (NULL == bstrMetaDataXML)
			return E_POINTER;
		*bstrMetaDataXML = CComBSTR(_GetXML().c_str()).Copy();
		return S_OK;
	}

	STDMETHOD(SetTranslateXML)(BSTR bstrTranslateXML)
	{
		// Add your function implementation here.
		return E_NOTIMPL;
	}
	
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		if (_bstr_t(ParamName) == _bstr_t(L"FLAG_RAWMAKERNOTE"))
		{
			if (ParamValue.vt != VT_BOOL)
				return E_INVALIDARG;

			if (VARIANT_TRUE == ParamValue.boolVal)
				m_dwTransformFlag |= AVS::Image::XML::MetaDataXMLTransform::TRANSFORM_RAWMAKERNOTETAGS;
			else
				m_dwTransformFlag &= ~AVS::Image::XML::MetaDataXMLTransform::TRANSFORM_RAWMAKERNOTETAGS;
			return S_OK;
		} else if (_bstr_t(ParamName) == _bstr_t(L"FLAG_USEUNKNOWN"))
		{
			if (ParamValue.vt != VT_BOOL)
				return E_INVALIDARG;

			if (VARIANT_TRUE == ParamValue.boolVal)
				m_dwTransformFlag |= AVS::Image::XML::MetaDataXMLTransform::TRANSFORM_USEUNKNOWN;
			else
				m_dwTransformFlag &= ~AVS::Image::XML::MetaDataXMLTransform::TRANSFORM_USEUNKNOWN;
			return S_OK;
		}else
			return E_INVALIDARG;
	}

	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT * ParamValue)
	{
		if (_bstr_t(ParamName) == _bstr_t(L"TypeName"))
		{
			(*ParamValue).vt = VT_BSTR;
			(*ParamValue).bstrVal = ::SysAllocString(L"EXIF");
			return S_OK;
		}
		return E_INVALIDARG;
	}

private:
	DWORD m_dwTransformFlag;

};

