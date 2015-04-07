/*!	\file	ImageMetaDataIPTC.h
*	\breif	Declaration of the CImageMetaDataIPTC
*/

#pragma once
#include "resource.h"       // main symbols
#include "IImageMetaData.h"
#include "metadatatypes.h"

#include "..\metadataparsers\iptcparser.h"
#include <string>
#include "..\XML\MetaDataTransform.h"
#include "MetaDataInterface.h"


/*!	\class	CImageMetaDataExif
*/
[
	coclass,
	default(IImageMetaDataIPTC),
	threading(apartment),
	support_error_info("IImageMetaDataIPTC"),
	vi_progid("AVSImageMetaData.ImageMetaDataIPTC"),
	progid("AVSImageMetaData.ImageMetaDataIPTC.1"),
	version(1.0),
	uuid("670B7133-278D-4935-A812-8744E2C2215C")
]
class ATL_NO_VTABLE CImageMetaDataIPTC :
	public IImageMetaDataIPTC,
	public AVS::Image::Details::ImageMetaDataCommonImpl
{
	friend class CImageMetaDataParser;
public:
	CImageMetaDataIPTC()
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
	// IImageMetaDataIPTC Methods
	// IImageMetaDataCommon Methods

	STDMETHOD(get_MetaDataType)(DWORD * dwMode)
	{
		if (NULL == dwMode)
			return E_POINTER;
		*dwMode = AVS::Image::MetaDataTypes::IPTC;
		return S_OK;
	}

	STDMETHOD(GetCommonXML)(DWORD dwMode, BSTR * bstrMetaDataXML)
	{
		HRESULT hr;
		if (NULL == bstrMetaDataXML)
			return E_POINTER;
		*bstrMetaDataXML = NULL;
			
		BSTR bsIPTCXml = NULL;
		if FAILED(hr = GetXML(0, &bsIPTCXml))
			return hr;

		BSTR bsResultXml = NULL;
		if (!AVS::Image::XML::MetaDataXMLTransform::TransformIPTCToCommon(bsIPTCXml, &bsResultXml))
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
		return E_NOTIMPL;
	}

	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT * ParamValue)
	{
		if (_bstr_t(ParamName) == _bstr_t(L"TypeName"))
		{
			(*ParamValue).vt = VT_BSTR;
			(*ParamValue).bstrVal = ::SysAllocString(L"IPTC");
			return S_OK;
		}
		return E_INVALIDARG;
	}
};

