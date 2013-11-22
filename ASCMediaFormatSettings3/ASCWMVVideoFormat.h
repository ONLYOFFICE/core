// ASCWMVVideoFormat.h : Declaration of the CASCWMVVideoFormat

#pragma once
#include "resource.h"       // main symbols
//#include "ATLDefine.h"
#include "ASCVideoFormat.h"


// IAVSWMVVideoFormat
[object, uuid("24DA753F-13AE-47E1-A86A-582DF4ECA9A6"), dual,	helpstring("IAVSWMVVideoFormat Interface"), pointer_default(unique)]
__interface IAVSWMVVideoFormat : IAVSVideoFormat
{
	[id(108000 + 0), propget] HRESULT HasSMPTE([out, retval] VARIANT_BOOL* pVal);
	[id(108000 + 0), propput] HRESULT HasSMPTE([in] VARIANT_BOOL newVal);
	[id(108000 + 1), propget] HRESULT BufferWindow([out, retval] long* pVal);
	[id(108000 + 1), propput] HRESULT BufferWindow([in] long newVal);
	[id(108000 + 2), propget] HRESULT Format([out, retval] long* pVal);
	[id(108000 + 2), propput] HRESULT Format([in] long newVal);
	[id(108000 + 3), propget] HRESULT Quality([out, retval] long* pVal);
	[id(108000 + 3), propput] HRESULT Quality([in] long newVal);
	[id(108000 + 4), propget] HRESULT IsVBR([out, retval] VARIANT_BOOL* pVal);
	[id(108000 + 4), propput] HRESULT IsVBR([in] VARIANT_BOOL newVal);
	[id(108000 + 5), propget] HRESULT Mode([out, retval] long* pVal);
	[id(108000 + 5), propput] HRESULT Mode([in] long newVal);
	[id(108000 + 6), propget] HRESULT MaxBitrate([out, retval] long* pVal);
	[id(108000 + 6), propput] HRESULT MaxBitrate([in] long newVal);
	[id(108000 + 7), propget] HRESULT MaxBuffer([out, retval] long* pVal);
	[id(108000 + 7), propput] HRESULT MaxBuffer([in] long newVal);
	[id(108000 + 8), propget] HRESULT KeyInterval([out, retval] long* pVal);
	[id(108000 + 8), propput] HRESULT KeyInterval([in] long newVal);
	[id(108000 + 9), propget] HRESULT VBRQuality([out, retval] long* pVal);
	[id(108000 + 9), propput] HRESULT VBRQuality([in] long newVal);
	//[id(108000 + 9), propget] HRESULT Language([out, retval] ULONG* pVal);
	//[id(108000 + 9), propput] HRESULT Language([in] ULONG newVal);
};

template <class CoT, class T>
class CAVSWMVVideoFormatImpl : public CAVSVideoFormatImpl<CoT, T>
{
public:
	CAVSWMVVideoFormatImpl()
	{
		m_lMediaType = MEDIAFORMAT_WMV;
		SetDefaultProperties();
	}
	~CAVSWMVVideoFormatImpl()
	{
	}
	STDMETHOD(CreateDuplicate)(IAVSMediaFormat** pMediaFormat)
	{
		CAVSVideoFormatImpl<CoT, T>::CreateDuplicate(pMediaFormat);
		if (NULL==(*pMediaFormat))
			return S_OK;

		IAVSWMVVideoFormat* pAVSWMVFormat=NULL;
		(*pMediaFormat)->QueryInterface(__uuidof(IAVSWMVVideoFormat), (void**)&pAVSWMVFormat);
		if (NULL==pAVSWMVFormat)
		{
			(*pMediaFormat)->Release();
			*pMediaFormat = NULL;
			return S_OK;
		}

		pAVSWMVFormat->put_HasSMPTE(m_HasSMPTE);
		pAVSWMVFormat->put_BufferWindow(m_BufferWindow);
		pAVSWMVFormat->put_Format(m_Format);
		pAVSWMVFormat->put_Quality(m_Quality);
		pAVSWMVFormat->put_IsVBR(m_IsVBR);
		pAVSWMVFormat->put_Mode(m_Mode);
		pAVSWMVFormat->put_MaxBitrate(m_MaxBitrate);
		pAVSWMVFormat->put_MaxBuffer(m_MaxBuffer);
		pAVSWMVFormat->put_KeyInterval(m_KeyInterval);
		pAVSWMVFormat->put_VBRQuality(m_VBRQuality);//Language(m_Language);
		
		pAVSWMVFormat->Release();

		return S_OK;
	}
	STDMETHOD(SetDefaultProperties)(void)
	{
		CAVSVideoFormatImpl<CoT, T>::SetDefaultProperties();

		m_HasSMPTE=FALSE;
		m_BufferWindow=3000;
		m_Format=0;
		m_Quality=100;
		m_IsVBR=FALSE;
		m_Mode=0;
		m_MaxBitrate=1000000;
		m_MaxBuffer=10000;
		m_KeyInterval=8;
		m_VBRQuality=100;
		//m_Language=1033;

		return S_OK;
	}
	STDMETHOD(SetPropertiesByFormat)(IAVSMediaFormat* pMediaFormat)
	{
		CAVSVideoFormatImpl<CoT, T>::SetPropertiesByFormat(pMediaFormat);
		if (pMediaFormat!=NULL)
		{
			IAVSWMVVideoFormat* pAVSWMVFormat=NULL;
			pMediaFormat->QueryInterface(__uuidof(IAVSWMVVideoFormat), (void**)&pAVSWMVFormat);
			if (pAVSWMVFormat!=NULL)
			{
				pAVSWMVFormat->get_HasSMPTE(&m_HasSMPTE);
				pAVSWMVFormat->get_BufferWindow(&m_BufferWindow);
				pAVSWMVFormat->get_Format(&m_Format);
				pAVSWMVFormat->get_Quality(&m_Quality);
				pAVSWMVFormat->get_IsVBR(&m_IsVBR);
				pAVSWMVFormat->get_Mode(&m_Mode);
				pAVSWMVFormat->get_MaxBitrate(&m_MaxBitrate);
				pAVSWMVFormat->get_MaxBuffer(&m_MaxBuffer);
				pAVSWMVFormat->get_KeyInterval(&m_KeyInterval);
				pAVSWMVFormat->get_VBRQuality(&m_VBRQuality);//Language(&m_Language);

				RELEASEINTERFACE(pAVSWMVFormat);
			}
		}
		return S_OK;
	}
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		//Здесь проверяем параметры относящиеся к AVSWMVVideoFormat
		return CAVSVideoFormatImpl<CoT, T>::SetAdditionalParam(ParamName, ParamValue);
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		//Здесь проверяем параметры относящиеся к AVSWMVVideoFormat
		return CAVSVideoFormatImpl<CoT, T>::GetAdditionalParam(ParamName, ParamValue);
	}

	LINK_PROPERTY_VARIANT_BOOL(HasSMPTE, m_HasSMPTE);
	LINK_PROPERTY_LONG(BufferWindow, m_BufferWindow);
	LINK_PROPERTY_LONG(Format, m_Format);
	LINK_PROPERTY_LONG(Quality, m_Quality);
	LINK_PROPERTY_VARIANT_BOOL(IsVBR, m_IsVBR);
	LINK_PROPERTY_LONG(Mode, m_Mode);
	LINK_PROPERTY_LONG(MaxBitrate, m_MaxBitrate);
	LINK_PROPERTY_LONG(MaxBuffer, m_MaxBuffer);
	LINK_PROPERTY_LONG(KeyInterval, m_KeyInterval);
	LINK_PROPERTY_LONG(VBRQuality, m_VBRQuality);
	//LINK_PROPERTY_ULONG(Language, m_Language);
protected:
	VARIANT_BOOL	m_HasSMPTE;
	LONG	m_BufferWindow;
	LONG	m_Format;
	LONG	m_Quality;
	VARIANT_BOOL	m_IsVBR;
	LONG	m_Mode;
	LONG	m_MaxBitrate;
	LONG	m_MaxBuffer;
	LONG	m_KeyInterval;
	//ULONG	m_Language;
	LONG	m_VBRQuality;
};

// CAVSWMVVideoFormat
[coclass, uuid("020D5105-06DE-4C30-BB9E-AFB9CE348554"), threading(apartment), vi_progid("AVSMediaFormatSettings3.AVSWMVVideoForm"), progid("AVSMediaFormatSettings3.AVSWMVVideoFo.1"), version(1.0)]
class ATL_NO_VTABLE CAVSWMVVideoFormat : public CAVSWMVVideoFormatImpl<CAVSWMVVideoFormat, IAVSWMVVideoFormat>
{
};
