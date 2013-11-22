// AVSWMAAudioFormat.h : Declaration of the CAVSWMAAudioFormat

#pragma once
#include "resource.h"       // main symbols
//#include "ATLDefine.h"
//#include "AVSAudioFormat.h"
#include "CodecsWMA.h"

// IAVSWMAAudioFormat
[
	object,
	uuid("9B5257BB-9D78-40E6-A5DF-BD20F9B205BF"),
	dual,	helpstring("IAVSWMAAudioFormat Interface"),
	pointer_default(unique)
]
__interface IAVSWMAAudioFormat : IAVSAudioFormat
{
	[propget, id(111000 + 1)] HRESULT ProfileCount([out, retval] long* pVal);
	[propget, id(111000 + 2)] HRESULT ProfileName([out, retval] BSTR* pVal);
	[propget, id(111000 + 3)] HRESULT ProfileNum([out, retval] long* pVal);
	[propput, id(111000 + 3)] HRESULT ProfileNum([in] long newVal);
	[propget, id(111000 + 4)] HRESULT ProfileData([out, retval] IUnknown** pVal);
};


template <class CoT, class T>
class CAVSWMAAudioFormatImpl : public CAVSAudioFormatImpl<CoT, T>
{
public:
	CAVSWMAAudioFormatImpl()
		: m_bQueryEnabled(TRUE)
		, m_lProfileNum(-1)
	{
		m_lMediaType = MEDIAFORMAT_WMA;
		SetDefaultProperties();
	}

	STDMETHOD(get_ProfileCount)(long* pVal)
	{
		*pVal = (long)m_arFilteredFormatIndex.GetCount();
		return S_OK;
	}
	STDMETHOD(get_ProfileNum)(long* pVal)
	{
		*pVal = m_lProfileNum;
		return S_OK;
	}
	STDMETHOD(get_ProfileName)(BSTR* pVal)
	{
		CString sProfileName;
		if (m_lProfileNum >= 0)
		{
			const SWMACodecData &oCodec = m_oCodecsManager[m_arFilteredFormatIndex[m_lProfileNum]];
			sProfileName = oCodec.m_sCodecName;
			sProfileName = + _T(" ");
			sProfileName = + oCodec.m_sFormatName;
		}
		*pVal = sProfileName.AllocSysString();
		return S_OK;
	}
	STDMETHOD(put_ProfileNum)(long newVal)
	{
		if (newVal < 0)
			newVal = 0;
		
		if (newVal >= (long)m_arFilteredFormatIndex.GetCount())
			newVal = (long)m_arFilteredFormatIndex.GetCount() - 1;
		
		if (newVal != m_lProfileNum)
		{
			m_lProfileNum = newVal;
			RELEASEARRAY(m_pExtraData);

			if (m_lProfileNum >= 0)
			{
				const SWMACodecData &oCodec = m_oCodecsManager[m_arFilteredFormatIndex[m_lProfileNum]];
				const WAVEFORMATEX *pFormatEx = oCodec.GetFormatEx();
				if (NULL!=pFormatEx)
				{
					m_pExtraData = SafeArrayCreateVector(VT_UI1, pFormatEx->cbSize);		
					memcpy(m_pExtraData->pvData, (LPBYTE)pFormatEx + sizeof(WAVEFORMATEX), pFormatEx->cbSize);
				}
			}				
		}
		return S_OK;
	}
	STDMETHOD(get_ProfileData)(IUnknown** pVal)
	{
		*pVal = NULL;
		if (m_lProfileNum < 0)
			return S_OK;

		const SWMACodecData &oCodec = m_oCodecsManager[m_arFilteredFormatIndex[m_lProfileNum]];
		if (!oCodec.IsFormatDataValid())
			m_oCodecsManager.InitFormatData(m_arFilteredFormatIndex[m_lProfileNum]);
		IWMStreamConfig *pTemp = NULL;
		oCodec.GetFormatData(&pTemp);
		if (NULL!=pTemp)
		{
			pTemp->QueryInterface(__uuidof(IUnknown), (void**)pVal);
			pTemp->Release();
		}
		return S_OK;
	}

	STDMETHOD(GetFormatHeader)(BYTE** ppData, long *pSize)
	{
		*ppData = NULL;
		*pSize = 0;
	
		if ((m_lProfileNum < 0) || (m_lProfileNum >= m_arFilteredFormatIndex.GetCount()))
			return S_FALSE;
		if (!m_lChannelsMask)	
			SetChannelsMaskPreset(m_lChannels, (DWORD&)m_lChannelsMask);
		else 
			CheckChannelsMask(m_lChannels, (DWORD&)m_lChannelsMask);
	
	
		const SWMACodecData &oCodec = m_oCodecsManager[m_arFilteredFormatIndex[m_lProfileNum]];
		const WAVEFORMATEX *pFormatEx = oCodec.GetFormatEx();

		*pSize = sizeof(WAVEFORMATEX) + pFormatEx->cbSize;
		*ppData = (BYTE*)::HeapAlloc(GetProcessHeap(), 0, *pSize);
		if (*ppData)
			memcpy(*ppData, pFormatEx, *pSize);
		return S_OK;
	}

	STDMETHOD(CreateDuplicate)(IAVSMediaFormat** pMediaFormat)
	{
		HRESULT	hResult = CoCreateInstance(__uuidof(CAVSWMAAudioFormat), NULL, CLSCTX_ALL, __uuidof(IAVSWMAAudioFormat), (void**)pMediaFormat);

		if (NULL==(*pMediaFormat))
			return S_OK;
	
		IAVSWMAAudioFormat* pAVSWMAFormat=NULL;
		(*pMediaFormat)->QueryInterface(__uuidof(IAVSWMAAudioFormat), (void**)&pAVSWMAFormat);
		if (NULL==pAVSWMAFormat)
		{
			(*pMediaFormat)->Release();
			*pMediaFormat = NULL;
			return S_OK;
		}
		VARIANT varVal;
		varVal.vt = VT_BOOL;
		varVal.boolVal=FALSE;
		(*pMediaFormat)->SetAdditionalParam(L"QueryEnabled",varVal);
	
		pAVSWMAFormat->put_FormatTag(m_lFormatTag);
		pAVSWMAFormat->put_SampleRate(m_lSampleRate);
		pAVSWMAFormat->put_Bitrate(m_lBitrate);
		pAVSWMAFormat->put_Channels(m_lChannels);
		pAVSWMAFormat->put_SampleSize(m_lSampleSize);

		pAVSWMAFormat->put_BlockAlign(m_lBlockAlign);
		pAVSWMAFormat->put_ChannelsMask(m_lChannelsMask);

		varVal.boolVal = TRUE;
		(*pMediaFormat)->SetAdditionalParam(L"QueryEnabled",varVal);
		pAVSWMAFormat->put_ProfileNum(m_lProfileNum);	
		pAVSWMAFormat->Release();
	
		return S_OK;
	}

	STDMETHOD(SetDefaultProperties)(void)
	{
		CAVSAudioFormatImpl<CoT, T>::SetDefaultProperties();
		
		//for all
		m_lFormatTag = 0;
		m_lSampleRate = 0;
		
		m_lChannels = 0;
		m_lSampleSize = 0;

		m_lBlockAlign = 0;
		m_lBitrate = 0;
		m_lChannelsMask = 0;
		
		QueryFormats();

		return S_OK;
	}
	STDMETHOD(SetPropertiesByFormat)(IAVSMediaFormat* pMediaFormat)
	{
		CAVSAudioFormatImpl<CoT, T>::SetPropertiesByFormat(pMediaFormat);
		if (pMediaFormat!=NULL)
		{
			IAVSWMAAudioFormat* pAVSWMAFormat=NULL;
			pMediaFormat->QueryInterface(__uuidof(IAVSWMAAudioFormat), (void**)&pAVSWMAFormat);
			if (pAVSWMAFormat!=NULL)
			{
				long newVal=0;
				pAVSWMAFormat->get_FormatTag(&newVal);
				put_FormatTag(newVal);
				pAVSWMAFormat->get_ProfileNum(&newVal);
				put_ProfileNum(newVal);
				RELEASEINTERFACE(pAVSWMAFormat);
			}
		}
		return S_OK;
	}
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		CString sParamName; sParamName = ParamName;
		if ("QueryEnabled" == sParamName)
		{
			m_bQueryEnabled = (VARIANT_FALSE != ParamValue.boolVal);
			QueryFormats();
		}
		return CAVSAudioFormatImpl<CoT, T>::SetAdditionalParam(ParamName, ParamValue);
	}

	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		//«десь провер€ем параметры относ€щиес€ к AVSWMAAudioFormat
		return CAVSAudioFormatImpl<CoT, T>::GetAdditionalParam(ParamName, ParamValue);
	}


	STDMETHOD(put_FormatTag)(long newVal)
	{
		CAVSAudioFormatImpl<CoT, T>::put_FormatTag(newVal);
	
		QueryFormats();
		return S_OK;
	}
	STDMETHOD(put_SampleRate)(long newVal)
	{
		CAVSAudioFormatImpl<CoT, T>::put_SampleRate(newVal);

		QueryFormats();
		return S_OK;
	}
	STDMETHOD(put_Bitrate)(long newVal)
	{
		CAVSAudioFormatImpl<CoT, T>::put_Bitrate(newVal);

		QueryFormats();
		return S_OK;
	}
	STDMETHOD(put_Channels)(long newVal)
	{
		CAVSAudioFormatImpl<CoT, T>::put_Channels(newVal);

		QueryFormats();
		return S_OK;
	}
	STDMETHOD(put_SampleSize)(long newVal)
	{
		CAVSAudioFormatImpl<CoT, T>::put_SampleSize(newVal);

		QueryFormats();
		return S_OK;
	}
	STDMETHOD(get_SampleSize)(long *pVal)
	{
		CAVSAudioFormatImpl<CoT, T>::get_SampleSize(pVal);
		if ((0 > m_lProfileNum) || ((long)m_arFilteredFormatIndex.GetCount() <= m_lProfileNum))
			return S_OK;
		*pVal = m_oCodecsManager[m_arFilteredFormatIndex[m_lProfileNum]].m_wSampleSize;
		return S_OK;
	}

	STDMETHOD(get_SampleRate)(long *pVal)
	{
		CAVSAudioFormatImpl<CoT, T>::get_SampleRate(pVal);
		if ((0 > m_lProfileNum) || ((long)m_arFilteredFormatIndex.GetCount() <= m_lProfileNum))
			return S_OK;
		*pVal = m_oCodecsManager[m_arFilteredFormatIndex[m_lProfileNum]].m_dwSampleRate;
		return S_OK;
	}
	STDMETHOD(get_Channels)(long *pVal)
	{
		CAVSAudioFormatImpl<CoT, T>::get_Channels(pVal);
		if ((0 > m_lProfileNum) || ((long)m_arFilteredFormatIndex.GetCount() <= m_lProfileNum))
			return S_OK;
		*pVal = m_oCodecsManager[m_arFilteredFormatIndex[m_lProfileNum]].m_wChannels;
		return S_OK;
	}
	STDMETHOD(get_ChannelsMask)(long *pVal)
	{
		CAVSAudioFormatImpl<CoT, T>::get_ChannelsMask(pVal);
		if ((0 > m_lProfileNum) || ((long)m_arFilteredFormatIndex.GetCount() <= m_lProfileNum))
			return S_OK;
		*pVal = m_oCodecsManager[m_arFilteredFormatIndex[m_lProfileNum]].m_dwChannelsMask;
		return S_OK;
	}
	STDMETHOD(get_Bitrate)(long *pVal)
	{
		CAVSAudioFormatImpl<CoT, T>::get_Bitrate(pVal);
		if ((0 > m_lProfileNum) || ((long)m_arFilteredFormatIndex.GetCount() <= m_lProfileNum))
			return S_OK;
		*pVal = m_oCodecsManager[m_arFilteredFormatIndex[m_lProfileNum]].m_dwBitrate;
		return S_OK;
	}

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
protected:
	BOOL m_bQueryEnabled;
	long m_lProfileNum;

	CWMACodecsManager m_oCodecsManager;
	CAtlArray<size_t> m_arFilteredFormatIndex;

	void QueryFormats()
	{
		if (!m_bQueryEnabled)
			return;

		m_arFilteredFormatIndex.RemoveAll(); 
		size_t nCodecsCount = m_oCodecsManager.GetCodecsCount();
		for(size_t nCodecIndex =0; nCodecIndex < nCodecsCount; nCodecIndex++)
		{			
			if ((0==m_oCodecsManager[nCodecIndex].m_dwSampleRate) || (m_oCodecsManager[nCodecIndex].m_dwBitrate<1000) || (0==m_oCodecsManager[nCodecIndex].m_wChannels))
				continue;

			if ((m_oCodecsManager[nCodecIndex].m_dwSampleRate != m_lSampleRate) && (0!=m_lSampleRate))
				continue;
			if ((m_oCodecsManager[nCodecIndex].m_wChannels!=m_lChannels) && (0!=m_lChannels))
				continue;
			if ((m_oCodecsManager[nCodecIndex].m_wSampleSize!=m_lSampleSize) && (0!=m_lSampleSize))
				continue;
			if ((m_oCodecsManager[nCodecIndex].m_dwBitrate/1000 != m_lBitrate/1000) && (0!=m_lBitrate/1000))
				continue;
			if ((m_oCodecsManager[nCodecIndex].m_wFormatTag != m_lFormatTag) && (0!=m_lFormatTag))
				continue;
			
			m_arFilteredFormatIndex.Add(nCodecIndex);
/*
			//Formats.Add(WMA.Codecs[nCodecIndex]);/// —транно но здесь падает при быстрой прокрутке пресетов
			Formats.Add();
			long ind = (long)Formats.GetCount()-1;
			{
				Formats[ind].Bitrate		= WMA.m_arCodecs[nCodecIndex].Bitrate;
				Formats[ind].Channels		= WMA.m_arCodecs[nCodecIndex].Channels;
				Formats[ind].ChannelsMask	= WMA.m_arCodecs[nCodecIndex].ChannelsMask;
				Formats[ind].CodecId		= WMA.m_arCodecs[nCodecIndex].CodecId;
				Formats[ind].CodecName		= WMA.m_arCodecs[nCodecIndex].CodecName.m_str;
				Formats[ind].FormatId		= WMA.m_arCodecs[nCodecIndex].FormatId;
				Formats[ind].FormatName		= WMA.m_arCodecs[nCodecIndex].FormatName.m_str;
				Formats[ind].FormatTag		= WMA.m_arCodecs[nCodecIndex].FormatTag;
				
				//TODO
				//Formats[ind].Num			= WMA.m_arCodecs[nCodecIndex].Num;
				Formats[ind].NumPasses		= WMA.m_arCodecs[nCodecIndex].NumPasses;
				Formats[ind].SampleRate		= WMA.m_arCodecs[nCodecIndex].SampleRate;
				Formats[ind].m_wSampleSize	= WMA.m_arCodecs[nCodecIndex].m_wSampleSize;
				Formats[ind].Vbr			= WMA.m_arCodecs[nCodecIndex].Vbr;
				Formats[ind].FormatData		= NULL;
				memcpy(Formats[ind].pFormat, WMA.m_arCodecs[nCodecIndex].pFormat, sizeof(WAVEFORMATEX)+WMA.m_arCodecs[nCodecIndex].pFormat->cbSize);
			}
*/
		}					
		put_ProfileNum(0);
	}
};

// CAVSWMAAudioFormat
[ coclass, uuid("3A28EE6F-900A-4ADE-8943-944112EEC0CD"), threading(apartment), vi_progid("AVSMediaFormatSettings3.AVSWMAAudioForm"), progid("AVSMediaFormatSettings3.AVSWMAAudioFo.1"), version(1.0)]
class ATL_NO_VTABLE CAVSWMAAudioFormat : public CAVSWMAAudioFormatImpl<CAVSWMAAudioFormat, IAVSWMAAudioFormat>
{
};