// AVSRMVideoFormat.h : Declaration of the CAVSRMAudioFormat

#pragma once
#include "resource.h"       // main symbols
//#include "ATLDefine.h"
//#include "AVSAudioFormat.h"


// IAVSRMAudioFormat
[object, uuid("7AD6A7C6-3EF5-4173-9ADD-9F8DF7EBBB1D"), dual,	helpstring("IAVSRMAudioFormat Interface"), pointer_default(unique)]
__interface IAVSRMAudioFormat : IAVSAudioFormat
{
	[id(110000 + 0), propget] HRESULT Flavor([out, retval] long* pVal);
	[id(110000 + 0), propput] HRESULT Flavor([in] long newVal);
	[id(110000 + 1), propget] HRESULT Complexity([out, retval] long* pVal);
	[id(110000 + 1), propput] HRESULT Complexity([in] long newVal);
	[id(110000 + 2), propget] HRESULT AudioMode([out, retval] long* pVal);
	[id(110000 + 2), propput] HRESULT AudioMode([in] long newVal);
	[id(110000 + 3), propget] HRESULT CodecIndex([out, retval] long* pVal);
	[id(110000 + 3), propput] HRESULT CodecIndex([in] long newVal);
	[id(110000 + 4), propget] HRESULT FormatIndex([out, retval] long* pVal);
	[id(110000 + 4), propput] HRESULT FormatIndex([in] long newVal);
};

template <class CoT, class T>
class CAVSRMAudioFormatImpl : public CAVSAudioFormatImpl<CoT, T>
{
public:
	CAVSRMAudioFormatImpl()
	{
		m_lMediaType = MEDIAFORMAT_RMAUDIO;
		SetDefaultProperties();
	}
	~CAVSRMAudioFormatImpl()
	{
	}
	STDMETHOD(CreateDuplicate)(IAVSMediaFormat** pMediaFormat)
	{
		CAVSAudioFormatImpl<CoT, T>::CreateDuplicate(pMediaFormat);
		if (NULL==(*pMediaFormat))
			return S_OK;

		IAVSRMAudioFormat* pAVSRMFormat=NULL;
		(*pMediaFormat)->QueryInterface(__uuidof(IAVSRMAudioFormat), (void**)&pAVSRMFormat);
		if (NULL==pAVSRMFormat)
		{
			(*pMediaFormat)->Release();
			*pMediaFormat = NULL;
			return S_OK;
		}

		pAVSRMFormat->put_Flavor(m_lFlavor);
		pAVSRMFormat->put_Complexity(m_lComplexity);
		pAVSRMFormat->put_AudioMode(m_lAudioMode);
		pAVSRMFormat->put_CodecIndex(m_lCodecIndex);
		pAVSRMFormat->put_FormatIndex(m_lFormatIndex);
		
		
		VARIANT vtCodecName;
		vtCodecName.vt = VT_BSTR;
		vtCodecName.bstrVal = m_sCodecName.AllocSysString();
		pAVSRMFormat->SetAdditionalParam(L"CodecName", vtCodecName);
		SysFreeString(vtCodecName.bstrVal);

		pAVSRMFormat->Release();

		return S_OK;
	}
	STDMETHOD(SetDefaultProperties)(void)
	{
		CAVSAudioFormatImpl<CoT, T>::SetDefaultProperties();

		m_lFlavor = 25;
		m_lComplexity = ENCODINGCOMPLEXITY_HIGH;
		m_lAudioMode = RMAUDIOMUSIC;
		m_lCodecIndex = 0;
		m_lFormatIndex = 0;
		m_sCodecName = _T("");

		return S_OK;
	}
	STDMETHOD(SetPropertiesByFormat)(IAVSMediaFormat* pMediaFormat)
	{
		CAVSAudioFormatImpl<CoT, T>::SetPropertiesByFormat(pMediaFormat);
		if (pMediaFormat!=NULL)
		{
			IAVSRMAudioFormat* pAVSRMFormat=NULL;
			pMediaFormat->QueryInterface(__uuidof(IAVSRMAudioFormat), (void**)&pAVSRMFormat);
			if (pAVSRMFormat!=NULL)
			{
				pAVSRMFormat->get_Flavor(&m_lFlavor);
				pAVSRMFormat->get_Complexity(&m_lComplexity);
				pAVSRMFormat->get_AudioMode(&m_lAudioMode);
				pAVSRMFormat->get_CodecIndex(&m_lCodecIndex);
				pAVSRMFormat->get_FormatIndex(&m_lFormatIndex);

				RELEASEINTERFACE(pAVSRMFormat);
			}
		}
		return S_OK;
	}
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		CString sParamName; sParamName = ParamName;
		if(_T("CodecName") == sParamName)
		{
			m_sCodecName = ParamValue.bstrVal;
		}
		return CAVSAudioFormatImpl<CoT, T>::SetAdditionalParam(ParamName, ParamValue);
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		//Здесь проверяем параметры относящиеся к AVSRMAudioFormat
		return CAVSAudioFormatImpl<CoT, T>::GetAdditionalParam(ParamName, ParamValue);
	}
	LINK_PROPERTY_LONG(Flavor, m_lFlavor);
	LINK_PROPERTY_LONG(Complexity, m_lComplexity);
	LINK_PROPERTY_LONG(AudioMode, m_lAudioMode);
	LINK_PROPERTY_LONG(CodecIndex, m_lCodecIndex);
	LINK_PROPERTY_LONG(FormatIndex, m_lFormatIndex);

	STDMETHOD(get_CodecName)(BSTR* pVal)
	{
		if (!m_sCodecName.IsEmpty())
		{
			*pVal = m_sCodecName.AllocSysString();
			return S_OK;
		}
		CString sCodecName = _T("");
		switch (m_lFormatTag)
		{
		case WAVE_FORMAT_RA_1:
			sCodecName = _T("RealAudio 1.0");
			break;
		case WAVE_FORMAT_RA_2://'2', '8', '_', '8'
			sCodecName = _T("RealAudio 2.0");
			break;
		case WAVE_FORMAT_RA_COOK://'c', 'o', 'o', 'k'
			sCodecName = _T("RealAudio COOK");
			break;
		case WAVE_FORMAT_RA_SIPRO://'s', 'i', 'p', 'r'
			sCodecName = _T("RealAudio Sipro");
			break;
		case WAVE_FORMAT_RA_ATRAC3://'a', 't', 'r', 'c'
			sCodecName = _T("RealAudio ATRAC3");
			break;
		case WAVE_FORMAT_RA_AC3://'d', 'n', 'e', 't'
			sCodecName = _T("RealAudio AC3");
			break;
		case WAVE_FORMAT_RA_AAC:
			//sCodecName = _T("RealAudio RALF");		//'r', 'a', 'l', 'f'
			sCodecName = _T("RealAudio AAC");		//'r', 'a', 'a', 'c'
			break;
		default:
			sCodecName = _T("Unknown");
			ATLASSERT(FALSE);
			break;
		}
		*pVal = sCodecName.AllocSysString();
		return S_OK;
	}


protected:
	CString		m_sCodecName;
	long		m_lFlavor;
	long		m_lComplexity;
	long		m_lAudioMode;
	long		m_lCodecIndex;
	long		m_lFormatIndex;
};

// CAVSRMAudioFormat
[coclass, uuid("4D72093A-32FB-48cc-973D-99EA63555032"), threading(apartment), vi_progid("AVSMediaFormatSettings3.AVSRMAudioForm"), progid("AVSMediaFormatSettings3.AVSRMAudioFo.1"), version(1.0)]
class ATL_NO_VTABLE CAVSRMAudioFormat : public CAVSRMAudioFormatImpl<CAVSRMAudioFormat, IAVSRMAudioFormat>
{
};
