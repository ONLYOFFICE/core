#pragma once

#include "ASCMediaData.h"
	
[object, uuid("31858644-1101-4825-A7FE-3AA261BA74AC"), dual, pointer_default(unique)]
__interface IAVSAudioData : IAVSMediaData
{
	[id(201000 + 1), propget] HRESULT FormatTag([out, retval] long* pVal);

	[id(201000 + 2), propget] HRESULT SampleRate([out, retval] long* pVal);
	[id(201000 + 3), propget] HRESULT Bitrate([out, retval] long* pVal);
	[id(201000 + 4), propget] HRESULT Channels([out, retval] long* pVal);
	[id(201000 + 5), propget] HRESULT SampleSize([out, retval] long* pVal);

	[id(201000 + 6), propget] HRESULT BlockAlign([out, retval] long* pVal);
	[id(201000 + 7), propget] HRESULT ChannelsMask([out, retval] long* pVal);
	[id(201000 + 7), propput] HRESULT ChannelsMask([in] long newVal);
};
	
template <class CoT, class T>
class CAVSAudioDataImpl : public CAVSMediaDataImpl<CoT, T>
{
protected:
	
	LONG m_lChannelsMask;
		
protected:
	
	WAVEFORMATEX* GetArrayFormatPtr()
	{
		return (WAVEFORMATEX*)(m_pMediaBuffer->GetBuffer());
	}
	
public:
	
	CAVSAudioDataImpl()
	{
		m_lMediaType = MEDIAFORMAT_AUDIO;
		m_lChannelsMask = 0x02;
	}
	
	STDMETHOD(CreateDuplicate)(long DublicateType, IAVSMediaData** pMediaData)
	{
		CAVSMediaDataImpl<CoT, T>::CreateDuplicate(DublicateType, pMediaData);
		if (NULL == *pMediaData)
			return S_OK;

		IAVSAudioData* pAVSAudioData = NULL;
		(*pMediaData)->QueryInterface(__uuidof(IAVSAudioData), (void**)(&pAVSAudioData));
		if (NULL == pAVSAudioData)
		{
			(*pMediaData)->Release();
			*pMediaData = NULL;
			return S_OK;
		}

		pAVSAudioData->put_ChannelsMask(m_lChannelsMask);
		RELEASEINTERFACE(pAVSAudioData)
		return S_OK;
	}
	
	STDMETHOD(get_FormatTag)(LONG* pVal)
	{
		*pVal = GetArrayFormatPtr()->wFormatTag;

		return S_OK;
	}
	STDMETHOD(get_SampleRate)(LONG* pVal)
	{
		*pVal = GetArrayFormatPtr()->nSamplesPerSec;

		return S_OK;
	}
	STDMETHOD(get_Bitrate)(LONG* pVal)
	{
		*pVal = GetArrayFormatPtr()->nAvgBytesPerSec*8;

		return S_OK;
	}
	STDMETHOD(get_Channels)(LONG* pVal)
	{
		*pVal = GetArrayFormatPtr()->nChannels;

		return S_OK;
	}
	STDMETHOD(get_SampleSize)(LONG* pVal)
	{
		*pVal = GetArrayFormatPtr()->wBitsPerSample;

		return S_OK;
	}
	STDMETHOD(get_BlockAlign)(LONG* pVal)
	{
		*pVal = GetArrayFormatPtr()->nBlockAlign;

		return S_OK;
	}
	STDMETHOD(get_ChannelsMask)(LONG* pVal)
	{
		*pVal = m_lChannelsMask;

		return S_OK;
	}
	STDMETHOD(put_ChannelsMask)(LONG newVal)
	{
		m_lChannelsMask = newVal;

		return S_OK;
	}
};
	
[coclass, uuid("3AC2E3B2-9B92-4006-AC78-A1A376F1A212"), threading(apartment), vi_progid("AVSMediaCore3.AVSAudioData"), progid("AVSMediaCore3.AVSAudioData.1"), version(1.0)]
class ATL_NO_VTABLE CAVSAudioData : public CAVSAudioDataImpl<CAVSAudioData, IAVSAudioData>
{
};

