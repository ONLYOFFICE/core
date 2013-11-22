// ASCMPEG4AudioFormat.h : Declaration of the CASCMPEG4AudioFormat

#pragma once
#include "resource.h"       // main symbols


// IAVSMPEG4AudioFormat
[
	object,
	uuid("047C86CD-492C-45B3-B65A-3B198C83206D"),
	dual,	helpstring("IAVSMPEG4AudioFormat Interface"),
	pointer_default(unique)
]
__interface IAVSMPEG4AudioFormat : IAVSAudioFormat
{
	[propget, id(112000 + 1)] HRESULT Quality([out, retval] long *pVal);
	[propput, id(112000 + 1)] HRESULT Quality([in] long newVal);
	[propget, id(112000 + 2)] HRESULT BandWidth([out, retval] long *pVal);
	[propput, id(112000 + 2)] HRESULT BandWidth([in] long newVal);
	[propget, id(112000 + 3)] HRESULT TNS([out, retval] VARIANT_BOOL *pVal);
	[propput, id(112000 + 3)] HRESULT TNS([in] VARIANT_BOOL newVal);
	[propget, id(112000 + 4)] HRESULT ProfileLevel([out, retval] long *pVal);
	[propput, id(112000 + 4)] HRESULT ProfileLevel([in] long newVal);
};

template <class CoT, class T>
class CAVSMPEG4AudioFormatImpl : public CAVSAudioFormatImpl<CoT, T>
{
public:
	CAVSMPEG4AudioFormatImpl()
	{
		m_lMediaType = MEDIAFORMAT_MPEG4AUDIO;
		SetDefaultProperties();
	}
	~CAVSMPEG4AudioFormatImpl()
	{
	}
	STDMETHOD(CreateDuplicate)(IAVSMediaFormat** pMediaFormat)
	{
		CAVSAudioFormatImpl<CoT, T>::CreateDuplicate(pMediaFormat);
		if (NULL==(*pMediaFormat))
			return S_OK;

		IAVSMPEG4AudioFormat* pAVSMPEG4Format=NULL;
		(*pMediaFormat)->QueryInterface(__uuidof(IAVSMPEG4AudioFormat), (void**)&pAVSMPEG4Format);
		if (NULL==pAVSMPEG4Format)
		{
			(*pMediaFormat)->Release();
			*pMediaFormat = NULL;
			return S_OK;
		}
		pAVSMPEG4Format->put_BandWidth(m_lBandWidth);
		pAVSMPEG4Format->put_ProfileLevel(m_lProfileLevel);
		pAVSMPEG4Format->put_Quality(m_lQuality);
		pAVSMPEG4Format->put_TNS(m_vbTNS);
		pAVSMPEG4Format->Release();

		return S_OK;
	}
	STDMETHOD(SetDefaultProperties)(void)
	{
		CAVSAudioFormatImpl<CoT, T>::SetDefaultProperties();

		m_lFormatTag=WAVE_FORMAT_AAC;
		m_lBitrate=128*1000;

		m_lQuality=100;
		m_lBandWidth=-1;
		m_vbTNS=FALSE;
		m_lProfileLevel=2;
		return S_OK;
	}
	STDMETHOD(SetPropertiesByFormat)(IAVSMediaFormat* pMediaFormat)
	{
		CAVSAudioFormatImpl<CoT, T>::SetPropertiesByFormat(pMediaFormat);
		if (pMediaFormat!=NULL)
		{
			IAVSMPEG4AudioFormat* pAVSMPEG4Format=NULL;
			pMediaFormat->QueryInterface(__uuidof(IAVSMPEG4AudioFormat), (void**)&pAVSMPEG4Format);
			if (pAVSMPEG4Format!=NULL)
			{
				pAVSMPEG4Format->get_BandWidth(&m_lBandWidth);
				pAVSMPEG4Format->get_ProfileLevel(&m_lProfileLevel);
				pAVSMPEG4Format->get_Quality(&m_lQuality);
				pAVSMPEG4Format->get_TNS(&m_vbTNS);
				RELEASEINTERFACE(pAVSMPEG4Format);
			}
		}
		return S_OK;
	}
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		//«десь провер€ем параметры относ€щиес€ к AVSMPEG4AudioFormat
		return CAVSAudioFormatImpl<CoT, T>::SetAdditionalParam(ParamName, ParamValue);
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		//«десь провер€ем параметры относ€щиес€ к AVSMPEG4AudioFormat
		return CAVSAudioFormatImpl<CoT, T>::GetAdditionalParam(ParamName, ParamValue);
	}
	STDMETHOD(GetFormatHeader)(BYTE** ppData, long* pSize)
	{
		*ppData = NULL;
		*pSize = 0;
	
		if (!m_lChannelsMask)	
			SetChannelsMaskPreset(m_lChannels,(DWORD&)m_lChannelsMask);
		else 
			CheckChannelsMask(m_lChannels,(DWORD&)m_lChannelsMask);


		if (m_lFormatTag == WAVE_APPLE_LOSSLESS) //костыль дл€ AudioConverter
		{
			(*pSize) = sizeof(WAVEFORMATEX)+4;
			if (NULL!=m_pExtraData)
				(*pSize) += m_pExtraData->rgsabound[0].cElements;

			*ppData = (BYTE*)HeapAlloc(GetProcessHeap(), 0, (*pSize));
			if (NULL == (*ppData))
			{
				(*pSize) = 0;
				return E_OUTOFMEMORY;
			}
			WAVEFORMATEX *pFormatEx = (WAVEFORMATEX *)(*ppData);
			
			pFormatEx->cbSize			= (WORD)((*pSize) - sizeof(WAVEFORMATEX));
			pFormatEx->wFormatTag		= WAVE_APPLE_LOSSLESS;
			pFormatEx->nChannels		= (WORD)m_lChannels;
			pFormatEx->nSamplesPerSec	= m_lSampleRate;
			pFormatEx->wBitsPerSample	= m_lSampleSize;
			pFormatEx->nBlockAlign		= (WORD)m_lChannels;
			pFormatEx->nAvgBytesPerSec = m_lSampleRate*m_lChannels;

			((DWORD*)(pFormatEx+1))[0] = m_lChannelsMask;

			if ((NULL!=m_pExtraData) && (0 < m_pExtraData->rgsabound[0].cElements))
				memcpy(((char*)(pFormatEx+1))+4, m_pExtraData->pvData, m_pExtraData->rgsabound[0].cElements);

		}	
		else //if (m_lFormatTag == WAVE_FORMAT_AAC)
		{
			(*pSize) = sizeof(WAVEFORMATEX_AAC);
			if (NULL!=m_pExtraData)
				(*pSize) += m_pExtraData->rgsabound[0].cElements;

			*ppData = (BYTE*)HeapAlloc(GetProcessHeap(), 0, (*pSize));
			if (NULL == *ppData)
			{
				(*pSize) = 0;
				return S_OK;
			}
			WAVEFORMATEX_AAC* pFormatEx = (WAVEFORMATEX_AAC*)(*ppData);

			WAVEFORMATEX &oFormat	= pFormatEx->wfx;
			oFormat.cbSize			= (WORD)((*pSize) - sizeof(WAVEFORMATEX));
			oFormat.wFormatTag		= WAVE_FORMAT_AAC;
			oFormat.nChannels		= (WORD)m_lChannels;
			oFormat.nSamplesPerSec	= m_lSampleRate;
			oFormat.wBitsPerSample	= 16;
			oFormat.nBlockAlign		= 1;
			oFormat.nAvgBytesPerSec = m_lBitrate/8;

			pFormatEx->wVersion			= 0;//mp4
			pFormatEx->wProfile			= (WORD)m_lProfileLevel;//lc
			pFormatEx->dwChannelMask	= m_lChannelsMask;
							
			if ((NULL!=m_pExtraData) && (0 < m_pExtraData->rgsabound[0].cElements))
				memcpy(pFormatEx->arnExtraData, m_pExtraData->pvData, m_pExtraData->rgsabound[0].cElements);	
		}

		return S_OK;
	}

	LINK_PROPERTY_LONG(ProfileLevel, m_lProfileLevel);
	LINK_PROPERTY_LONG(Quality, m_lQuality);
	LINK_PROPERTY_LONG(BandWidth, m_lBandWidth);
	LINK_PROPERTY_BOOL(TNS, m_vbTNS);

protected:
	long m_lQuality;
	long m_lBandWidth;
	VARIANT_BOOL m_vbTNS;
	long m_lProfileLevel;
};


// CAVSMPEG4AudioFormat

[ coclass, uuid("B254AEE5-E2D4-4F89-87D6-C7AEBD43B298"), threading(apartment), vi_progid("AVSMediaFormatSettings3.AVSMPEG4AudioFo"), progid("AVSMediaFormatSettings3.AVSMPEG4Audio.1"), version(1.0)]
class ATL_NO_VTABLE CAVSMPEG4AudioFormat : public CAVSMPEG4AudioFormatImpl<CAVSMPEG4AudioFormat, IAVSMPEG4AudioFormat>
{
};