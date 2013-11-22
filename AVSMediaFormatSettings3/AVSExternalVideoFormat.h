// AVSExternalVideoFormat.h : Declaration of the CAVSExternalVideoFormat

#pragma once
#include "resource.h"       // main symbols
#include "vfw.h"


#include "ATLDefine.h"
//#include "AVSVideoFormat.h"

class CAVSExternalVideoFormat;

// IAVSExternalVideoFormat
[object,uuid("F955F716-8FC4-4026-9B74-4C377C4E3C70"),dual,pointer_default(unique)]
__interface IAVSExternalVideoFormat : IAVSVideoFormat
{
	[id(114000 + 1), propget] HRESULT ExtraData([out, retval, satype("byte")] SAFEARRAY **pExtraData);
	[id(114000 + 1), propput] HRESULT ExtraData([in, satype("byte")] SAFEARRAY **pExtraData);
	[id(114000 + 2), propget] HRESULT KeyFrameInterval([out, retval] long* pVal);
	[id(114000 + 2), propput] HRESULT KeyFrameInterval([in] long newVal);
	[id(114000 + 3), propget] HRESULT Quality([out, retval] long* pVal);
	[id(114000 + 3), propput] HRESULT Quality([in] long newVal);
	
	[id(114000 + 5)]		  HRESULT Configure([in,defaultvalue(NULL)]long* hWND,[in,out,defaultvalue(NULL)] RECT* RectWindow);
	
};



// CAVSExternalVideoFormat

template <class CoT, class T>
class CAVSExternalVideoFormatImpl : public CAVSVideoFormatImpl<CoT, T>
{
public:
	CAVSExternalVideoFormatImpl()
	{
		m_Quality=0;
		m_KeyFrameInterval=0;
		m_pExtraData=NULL;
		m_lMediaType = MEDIAFORMAT_EXTERNAL;
		m_lColorSpace = CSP_BGR;
		SetDefaultProperties();
	}

	virtual ~CAVSExternalVideoFormatImpl()
	{
		RELEASEARRAY(m_pExtraData);
	}

	STDMETHOD(Configure)(long *hWnd,RECT* RectWindow)
	{
		HIC	hic=ICOpen(ICTYPE_VIDEO, m_ulfccHandler, ICMODE_QUERY);
		if (hic==NULL) 
			return S_OK;

		if (hWnd==NULL)
			hWnd=(long*)GetDesktopWindow();
		if (ICConfigure(hic,(HWND) hWnd)!=ICERR_OK)
		{
			ICClose(hic);
			return S_OK;
		}
		/////////////////////////////
		////ƒополнительные параметры - могут использоватьс€ в пресетах и...

		RELEASEARRAY(m_pExtraData);
		long DataSize = ICGetStateSize(hic); 
		m_pExtraData = SafeArrayCreateVector(VT_UI1,0,DataSize);

		ICGetState(hic, m_pExtraData->pvData, DataSize);

		ICClose(hic);
		return S_OK;
	}
		
	LINK_PROPERTY_LONG(Quality,m_Quality);
	LINK_PROPERTY_LONG(KeyFrameInterval,m_KeyFrameInterval);

	STDMETHOD(get_ExtraData)(SAFEARRAY **pExtraData)
	{
		if (NULL==pExtraData)
			return S_OK;

		SafeArrayCopy(m_pExtraData, pExtraData);
		return S_OK;
	}
	STDMETHOD(put_ExtraData)(SAFEARRAY **pExtraData)
	{
		if (NULL==pExtraData)
			return S_OK;

		RELEASEARRAY(m_pExtraData);

		if (NULL!=(*pExtraData))
			SafeArrayCopy(*pExtraData, &m_pExtraData);
		return S_OK;
	}
	STDMETHOD(CreateDuplicate)(IAVSMediaFormat** pMediaFormat)
	{
		CAVSVideoFormatImpl<CoT, T>::CreateDuplicate(pMediaFormat);
		if ((*pMediaFormat)!=NULL)
		{
			IAVSExternalVideoFormat * pExternalFormat=NULL;
			(*pMediaFormat)->QueryInterface(__uuidof(IAVSExternalVideoFormat), (void**)&pExternalFormat);
			if (NULL==pExternalFormat)
			{
				RELEASEINTERFACE((*pMediaFormat));
				return S_OK;
			}
			if (NULL==m_pExtraData)
				pExternalFormat->put_ExtraData(NULL);
			else
				pExternalFormat->put_ExtraData(&m_pExtraData);

			pExternalFormat->put_Quality(m_Quality);
			pExternalFormat->put_KeyFrameInterval(m_KeyFrameInterval);

			pExternalFormat->Release();
		}
		return S_OK;
	}

	STDMETHOD(put_fccHandler)(ULONG newVal)
	{
		CAVSVideoFormatImpl<CoT, T>::put_fccHandler(newVal);
		
		RELEASEARRAY(m_pExtraData);
		if (m_lInternalCodecType!=0)return S_OK;

		/// fill ExtraData
		HIC	hic=ICOpen(ICTYPE_VIDEO, m_ulfccHandler, ICMODE_QUERY);
		if (hic==NULL)	return S_OK;

		long DataSize = ICGetStateSize(hic); 
		m_pExtraData = SafeArrayCreateVector(VT_UI1,0,DataSize);

		ICGetState(hic, m_pExtraData->pvData, DataSize);

		ICClose(hic);
		return S_OK;
	}

	STDMETHOD(SetDefaultProperties)(void)
	{
		m_Quality=0;
		m_KeyFrameInterval=0;

		RELEASEARRAY(m_pExtraData);
		CAVSVideoFormatImpl<CoT, T>::SetDefaultProperties();

		return S_OK;
	}
	STDMETHOD(SetPropertiesByFormat)(IAVSMediaFormat* pMediaFormat)
	{
		RELEASEARRAY(m_pExtraData);

		CAVSVideoFormatImpl<CoT, T>::SetPropertiesByFormat(pMediaFormat);
		if (pMediaFormat!=NULL)
		{
			IAVSExternalVideoFormat*	pAVSExternalFormat=NULL;
			pMediaFormat->QueryInterface(__uuidof(IAVSH263VideoFormat), (void**)&pAVSExternalFormat);
			if (pAVSExternalFormat!=NULL)
			{
				RELEASEARRAY(m_pExtraData);
				pAVSExternalFormat->get_ExtraData(&m_pExtraData);

				pAVSExternalFormat->get_KeyFrameInterval(&m_KeyFrameInterval);
				pAVSExternalFormat->get_Quality(&m_Quality);


				RELEASEINTERFACE(pAVSExternalFormat);
			}
		}
		return S_OK;
	}
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		//«десь провер€ем параметры относ€щиес€ к AVSH263VideoFormat
		return CAVSVideoFormatImpl<CoT, T>::SetAdditionalParam(ParamName, ParamValue);
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		//«десь провер€ем параметры относ€щиес€ к AVSH263VideoFormat
		return CAVSVideoFormatImpl<CoT, T>::GetAdditionalParam(ParamName, ParamValue);
	}

public:
protected:
	LPSAFEARRAY m_pExtraData;
	long m_KeyFrameInterval;
	long m_Quality;


};

[
	coclass,
	threading(apartment),
	vi_progid("AVSMediaFormatSettings3.AVSExternalVide"),
	progid("AVSMediaFormatSettings3.AVSExternalVi.1"),
	version(1.0),
	uuid("9AFD5DEF-68D0-4864-9E8C-61764F487B8E")
]
class ATL_NO_VTABLE CAVSExternalVideoFormat : 	
	public CAVSExternalVideoFormatImpl<CAVSExternalVideoFormat, IAVSExternalVideoFormat>
{
};

