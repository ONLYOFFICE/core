// AVSVideoFormat.h : Declaration of the CAVSVideoFormat

#pragma once
#include "resource.h"       // main symbols
#include "AVSMediaFormat.h"       // main symbols


class CAVSSubPictureFormat;
// IAVSVideoFormat
[ object, uuid("1145BF83-7F69-4b44-8A31-3679C17B3DFD"), dual, pointer_default(unique)]
__interface IAVSSubPictureFormat : IAVSMediaFormat
{
	[id(118000 + 1), propget] HRESULT Buffer([out, retval] IUnknown** ppVal);
	[id(118000 + 1), propput] HRESULT Buffer([in] IUnknown* newVal);		
};

template <class CoT, class T>
class CAVSSubPictureFormatImpl : public CAVSMediaFormatImpl<CoT, T>
{
public:
	CAVSSubPictureFormatImpl()
	{
		m_lMediaType = MEDIAFORMAT_DVD_SUBPICTURE;
		SetDefaultProperties();
	}
	virtual ~CAVSSubPictureFormatImpl()
	{
		RELEASEINTERFACE(m_punkBuffer);
	}

	STDMETHOD(CreateDuplicate)(IAVSMediaFormat** pMediaFormat)
	{
		CAVSMediaFormatImpl<CoT, T>::CreateDuplicate(pMediaFormat);
		if (NULL==(*pMediaFormat))
			return S_OK;
		IAVSSubPictureFormat * pSPFormat=NULL;
		(*pMediaFormat)->QueryInterface(__uuidof(IAVSSubPictureFormat), (void**)&pSPFormat);
		if (NULL==pSPFormat)
		{
			(*pMediaFormat)->Release();
			*pMediaFormat = NULL;
			return S_OK;
		}

		pSPFormat->put_Buffer(m_punkBuffer);
		pSPFormat->Release();
		return S_OK;
	}
	STDMETHOD(SetDefaultProperties)(void)
	{
		m_punkBuffer = NULL;
		return S_OK;
	}
	STDMETHOD(SetPropertiesByFormat)(IAVSMediaFormat* pMediaFormat)
	{
		CAVSMediaFormatImpl<CoT, T>::SetPropertiesByFormat(pMediaFormat);
		if (pMediaFormat!=NULL)
		{
			IAVSSubPictureFormat * pSPFormat=NULL;
			pMediaFormat->QueryInterface(__uuidof(IAVSSubPictureFormat), (void**)&pSPFormat);
			if (NULL != pSPFormat)
			{
				RELEASEINTERFACE(m_punkBuffer);
				pSPFormat->get_Buffer(&m_punkBuffer);
				RELEASEINTERFACE(pSPFormat);
			}
		}
		return S_OK;
	}
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		//Здесь проверяем параметры относящиеся к VideoFormat
		return CAVSMediaFormatImpl<CoT, T>::SetAdditionalParam(ParamName, ParamValue);
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		//Здесь проверяем параметры относящиеся к VideoFormat
		return CAVSMediaFormatImpl<CoT, T>::GetAdditionalParam(ParamName, ParamValue);
	}	
	STDMETHOD(get_Buffer)(IUnknown** ppVal)
	{
		ADDREFINTERFACE(m_punkBuffer);
		*ppVal = m_punkBuffer;
		return S_OK;
	}
	STDMETHOD(put_Buffer)(IUnknown* pVal)
	{
		RELEASEINTERFACE(m_punkBuffer);
		m_punkBuffer = pVal;
		ADDREFINTERFACE(m_punkBuffer);
		return S_OK;
	}
protected:	
	IUnknown *m_punkBuffer;
};


// CAVSVideoFormat
[coclass, uuid("1C6B1185-E32A-4e66-9D78-624FD99A07B4"), threading(apartment), vi_progid("AVSMediaFormatSettings3.SubPicture"), progid("AVSMediaFormatSettings3.SubPicture.1"), version(1.0)]
class ATL_NO_VTABLE CAVSSubPictureFormat : public CAVSSubPictureFormatImpl<CAVSSubPictureFormat, IAVSSubPictureFormat>
{
};

