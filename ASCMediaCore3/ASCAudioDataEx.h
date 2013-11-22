#pragma once

#include "ASCMediaData.h"
	
[object, uuid("35B7E717-3EF0-4473-884D-A42C41E80AE1"), dual, pointer_default(unique)]
__interface IAVSAudioDataEx : IAVSMediaData
{

	[id(210000 + 1), propget] HRESULT FormatTag([out, retval] long* pVal);

	[id(210000 + 2), propget] HRESULT SampleRate([out, retval] long* pVal);
	[id(210000 + 3), propget] HRESULT Bitrate([out, retval] long* pVal);
	[id(210000 + 4), propget] HRESULT Channels([out, retval] long* pVal);
	[id(210000 + 5), propget] HRESULT SampleSize([out, retval] long* pVal);

	[id(210000 + 6), propget] HRESULT BlockAlign([out, retval] long* pVal);
	[id(210000 + 7), propget] HRESULT ChannelsMask([out, retval] long* pVal);
	[id(210000 + 7), propput] HRESULT ChannelsMask([in] long newVal);
	
	[id(210000 + 8)] HRESULT AllocateHeader([in] long HeaderSize);
	[id(210000 + 9), propget] HRESULT Header([out, retval] BYTE** pVal);
	[id(210000 + 10), propget] HRESULT HeaderSize([out, retval] long* pVal);
};
	
template <class CoT, class T>
class CAVSAudioDataExImpl : public CAVSMediaDataImpl<CoT, T>
{
protected:
	
	LONG m_lChannelsMask;
	CMediaBuffer* m_pAudioHeader;
		
protected:
	
	WAVEFORMATEX* GetArrayFormatPtr()
	{
		return (WAVEFORMATEX*)(m_pAudioHeader->GetBuffer());
	}
	
public:
	
	CAVSAudioDataExImpl()
	{
		m_lMediaType = MEDIAFORMAT_AUDIO;
		m_lChannelsMask = 0x03; //FL+FR
		m_pAudioHeader = new CMediaBuffer();
	}
	~CAVSAudioDataExImpl(void)
	{
		RELEASEINTERFACE(m_pAudioHeader)
	}
	
	STDMETHOD(AllocateHeader)(LONG HeaderSize)
	{
		m_pAudioHeader->SetBuffer(HeaderSize);
		return S_OK;
	}
	STDMETHOD(get_Header)(BYTE** pVal)
	{
		*pVal = 0;
		if (NULL!=m_pAudioHeader)
			*pVal = m_pAudioHeader->GetBuffer();
		return S_OK;
	}
	STDMETHOD(get_HeaderSize)(LONG* pVal)
	{
		*pVal = 0;
		if (NULL!=m_pAudioHeader)
			*pVal = m_pAudioHeader->GetBufferSize();
		return S_OK;
	}	
	STDMETHOD(CreateDuplicate)(long DublicateType, IAVSMediaData** pMediaData)
	{
		CAVSMediaDataImpl<CoT, T>::CreateDuplicate(DublicateType, pMediaData);
		if (NULL == *pMediaData)
			return S_OK;

		IAVSAudioDataEx* pAVSAudioData = NULL;
		(*pMediaData)->QueryInterface(__uuidof(IAVSAudioData), (void**)(&pAVSAudioData));
		if (NULL == pAVSAudioData)
		{
			(*pMediaData)->Release();
			*pMediaData = NULL;
			return S_OK;
		}
		//Set Audio Header

		pAVSAudioData->put_ChannelsMask(m_lChannelsMask);
		
		pAVSAudioData->AllocateHeader(m_pAudioHeader->GetBufferSize());
		BYTE *Header=NULL;
		pAVSAudioData->get_Header(&Header);
		if (Header)
			memcpy(Header,m_pAudioHeader->GetBuffer(),m_pAudioHeader->GetBufferSize());
		
		RELEASEINTERFACE(pAVSAudioData)
		return S_OK;
	}
	
	STDMETHOD(get_FormatTag)(LONG* pVal)
	{
		if (m_pAudioHeader->GetBuffer())
			*pVal = GetArrayFormatPtr()->wFormatTag;
		else
			*pVal=0;

		return S_OK;
	}
	STDMETHOD(get_SampleRate)(LONG* pVal)
	{
		if (m_pAudioHeader->GetBuffer())
			*pVal = GetArrayFormatPtr()->nSamplesPerSec;
		else
			*pVal=0;

		return S_OK;
	}
	STDMETHOD(get_Bitrate)(LONG* pVal)
	{
		if (m_pAudioHeader->GetBuffer())
			*pVal = GetArrayFormatPtr()->nAvgBytesPerSec*8;
		else
			*pVal=0;

		return S_OK;
	}
	STDMETHOD(get_Channels)(LONG* pVal)
	{
		if (m_pAudioHeader->GetBuffer())
			*pVal = GetArrayFormatPtr()->nChannels;
		else
			*pVal=0;

		return S_OK;
	}
	STDMETHOD(get_SampleSize)(LONG* pVal)
	{
		if (m_pAudioHeader->GetBuffer())
			*pVal = GetArrayFormatPtr()->wBitsPerSample;
		else
			*pVal=0;

		return S_OK;
	}
	STDMETHOD(get_BlockAlign)(LONG* pVal)
	{
		if (m_pAudioHeader->GetBuffer())
			*pVal = GetArrayFormatPtr()->nBlockAlign;
		else
			*pVal=0;

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
	
[coclass, uuid("181B5438-55CF-4bbb-82C7-F20BA8F032B3"), threading(apartment), vi_progid("AVSMediaCore3.AVSAudioDataEx"), progid("AVSMediaCore3.AVSAudioDataEx.1"), version(1.0)]
class ATL_NO_VTABLE CAVSAudioDataEx : public CAVSAudioDataExImpl<CAVSAudioDataEx, IAVSAudioDataEx>
{
};

