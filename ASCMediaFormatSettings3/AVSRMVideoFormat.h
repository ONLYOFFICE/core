// AVSRMVideoFormat.h : Declaration of the CAVSRMVideoFormat

#pragma once
#include "resource.h"       // main symbols
#include "ATLDefine.h"
#include "AVSVideoFormat.h"


// IAVSRMVideoFormat
[object, uuid("574E82A3-3525-4bbd-859D-F61CA55F9B07"), dual,	helpstring("IAVSRMVideoFormat Interface"), pointer_default(unique)]
__interface IAVSRMVideoFormat : IAVSVideoFormat
{
	[id(109000 + 0), propget] HRESULT MaxBitrate([out, retval] long* pVal);
	[id(109000 + 0), propput] HRESULT MaxBitrate([in] long newVal);
	[id(109000 + 1), propget] HRESULT EncodingType([out, retval] long* pVal);
	[id(109000 + 1), propput] HRESULT EncodingType([in] long newVal);
	[id(109000 + 2), propget] HRESULT Complexity([out, retval] long* pVal);
	[id(109000 + 2), propput] HRESULT Complexity([in] long newVal);
	[id(109000 + 3), propget] HRESULT Quality([out, retval] long* pVal);
	[id(109000 + 3), propput] HRESULT Quality([in] long newVal);
	[id(109000 + 4), propget] HRESULT MaxStartupLatency([out, retval] double* pVal);
	[id(109000 + 4), propput] HRESULT MaxStartupLatency([in] double newVal);
	[id(109000 + 5), propget] HRESULT MaxKeyFrameInterval([out, retval] double* pVal);
	[id(109000 + 5), propput] HRESULT MaxKeyFrameInterval([in] double newVal);
	[id(109000 + 6), propget] HRESULT EnableLossProtection([out, retval] VARIANT_BOOL* pVal);
	[id(109000 + 6), propput] HRESULT EnableLossProtection([in] VARIANT_BOOL newVal);
	[id(109000 + 7), propget] HRESULT VideoMode([out, retval] long* pVal);
	[id(109000 + 7), propput] HRESULT VideoMode([in] long newVal);
};

template <class CoT, class T>
class CAVSRMVideoFormatImpl : public CAVSVideoFormatImpl<CoT, T>
{
public:
	CAVSRMVideoFormatImpl()
	{
		m_lMediaType = MEDIAFORMAT_RMVIDEO;
		SetDefaultProperties();
	}
	~CAVSRMVideoFormatImpl()
	{
	}
	STDMETHOD(CreateDuplicate)(IAVSMediaFormat** pMediaFormat)
	{
		CAVSVideoFormatImpl<CoT, T>::CreateDuplicate(pMediaFormat);
		if (NULL==(*pMediaFormat))
			return S_OK;

		IAVSRMVideoFormat* pAVSRMFormat=NULL;
		(*pMediaFormat)->QueryInterface(__uuidof(IAVSRMVideoFormat), (void**)&pAVSRMFormat);
		if (NULL==pAVSRMFormat)
		{
			(*pMediaFormat)->Release();
			*pMediaFormat = NULL;
			return S_OK;
		}

		pAVSRMFormat->put_MaxBitrate(m_MaxBitrate);
		pAVSRMFormat->put_EncodingType(m_EncodingType);
		pAVSRMFormat->put_Complexity(m_Complexity);
		pAVSRMFormat->put_Quality(m_Quality);
		pAVSRMFormat->put_MaxStartupLatency(m_MaxStartupLatency);
		pAVSRMFormat->put_MaxKeyFrameInterval(m_MaxKeyFrameInterval);
		pAVSRMFormat->put_EnableLossProtection(m_EnableLossProtection);
		pAVSRMFormat->put_VideoMode(m_VideoMode);

		pAVSRMFormat->Release();

		return S_OK;
	}
	STDMETHOD(SetDefaultProperties)(void)
	{
		CAVSVideoFormatImpl<CoT, T>::SetDefaultProperties();

		m_MaxBitrate=m_lBitrate+30*m_lBitrate/100;
		m_EncodingType=ENCODINGTYPE_CBR;
		m_Complexity=ENCODINGCOMPLEXITY_HIGH;
		m_Quality=100;
		m_MaxStartupLatency=4;
		m_MaxKeyFrameInterval=10;
		m_EnableLossProtection=VARIANT_FALSE;
		m_VideoMode=VIDEOMODENORMAL;

		return S_OK;
	}
	STDMETHOD(SetPropertiesByFormat)(IAVSMediaFormat* pMediaFormat)
	{
		CAVSVideoFormatImpl<CoT, T>::SetPropertiesByFormat(pMediaFormat);
		if (pMediaFormat!=NULL)
		{
			IAVSRMVideoFormat*	pAVSRMFormat=NULL;
			pMediaFormat->QueryInterface(__uuidof(IAVSRMVideoFormat), (void**)&pAVSRMFormat);
			if (pAVSRMFormat!=NULL)
			{
				pAVSRMFormat->get_MaxBitrate(&m_MaxBitrate);
				pAVSRMFormat->get_EncodingType(&m_EncodingType);
				pAVSRMFormat->get_Complexity(&m_Complexity);
				pAVSRMFormat->get_Quality(&m_Quality);
				pAVSRMFormat->get_MaxStartupLatency(&m_MaxStartupLatency);
				pAVSRMFormat->get_MaxKeyFrameInterval(&m_MaxKeyFrameInterval);
				pAVSRMFormat->get_EnableLossProtection(&m_EnableLossProtection);
				pAVSRMFormat->get_VideoMode(&m_VideoMode);

				RELEASEINTERFACE(pAVSRMFormat);
			}
		}
		return S_OK;
	}
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		//Здесь проверяем параметры относящиеся к AVSRMVideoFormat
		return CAVSVideoFormatImpl<CoT, T>::SetAdditionalParam(ParamName, ParamValue);
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		//Здесь проверяем параметры относящиеся к AVSRMVideoFormat
		return CAVSVideoFormatImpl<CoT, T>::GetAdditionalParam(ParamName, ParamValue);
	}

	LINK_PROPERTY_LONG(MaxBitrate, m_MaxBitrate);
	LINK_PROPERTY_LONG(EncodingType, m_EncodingType);
	LINK_PROPERTY_LONG(Complexity, m_Complexity);
	LINK_PROPERTY_LONG(Quality, m_Quality);
	LINK_PROPERTY_DOUBLE(MaxStartupLatency, m_MaxStartupLatency);
	LINK_PROPERTY_DOUBLE(MaxKeyFrameInterval, m_MaxKeyFrameInterval);
	LINK_PROPERTY_VARIANT_BOOL(EnableLossProtection, m_EnableLossProtection);
	LINK_PROPERTY_LONG(VideoMode, m_VideoMode);
protected:
	LONG			m_MaxBitrate;
	LONG			m_EncodingType;
	LONG			m_Complexity;
	LONG			m_Quality;
	DOUBLE			m_MaxStartupLatency;
	DOUBLE			m_MaxKeyFrameInterval;
	VARIANT_BOOL	m_EnableLossProtection;
	LONG			m_VideoMode;
};

// CAVSRMVideoFormat
[coclass, uuid("4B89148B-1A8C-4884-8886-4895C8B0150D"), threading(apartment), vi_progid("AVSMediaFormatSettings3.AVSRMVideoForm"), progid("AVSMediaFormatSettings3.AVSRMVideoFo.1"), version(1.0)]
class ATL_NO_VTABLE CAVSRMVideoFormat : public CAVSRMVideoFormatImpl<CAVSRMVideoFormat, IAVSRMVideoFormat>
{
};
