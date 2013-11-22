#pragma once

#include "ASCAudioData.h"
	
[object, uuid("1B92459A-0727-491b-9E7F-FB0D1AA81F7B"), dual, pointer_default(unique)]
__interface IAVSPacketsAudioData : IAVSAudioData
{
	[id(212000 + 1), propget] HRESULT PacketsCount([out, retval] long* pVal);

	[id(212000 + 2), propget] HRESULT PacketOffset([in] long Index, [out, retval] long* pVal);
	
	[id(212000 + 3)]			HRESULT AddPacket([in] long Offset);
	[id(212000 + 4)]			HRESULT InsertPacket([in] long Index, [in] long Offset);
	[id(212000 + 5)]			HRESULT RemovePacket([in] long Index);
	[id(212000 + 6)]			HRESULT RemovePacketAll();
};
	
template <class CoT, class T>
class CAVSPacketsAudioDataImpl : public CAVSAudioDataImpl<CoT, T>
{
public:
	
	CAVSPacketsAudioDataImpl()
	{
	}
	
	STDMETHOD(CreateDuplicate)(long DublicateType, IAVSMediaData** pMediaData)
	{
		CAVSAudioDataImpl<CoT, T>::CreateDuplicate(DublicateType, pMediaData);
		if (NULL == *pMediaData)
			return S_OK;

		IAVSPacketsAudioData* pAVSPacketsAudioData = NULL;
		(*pMediaData)->QueryInterface(__uuidof(IAVSPacketsAudioData), (void**)(&pAVSPacketsAudioData));
		if (NULL == pAVSPacketsAudioData)
		{
			(*pMediaData)->Release();
			*pMediaData = NULL;
			return S_OK;
		}

		size_t nCount = m_arOffsets.GetCount();
		for (size_t nIndex = 0; nIndex < nCount; nIndex++)
		{
			pAVSPacketsAudioData->AddPacket(m_arOffsets[nIndex]);
		}		
		RELEASEINTERFACE(pAVSPacketsAudioData)
		return S_OK;
	}
	
	STDMETHOD(get_PacketsCount)(long* pVal)
	{
		*pVal = (long)m_arOffsets.GetCount();
		return S_OK;
	}

	STDMETHOD(get_PacketOffset)(long Index, long* pVal)
	{
		*pVal = 0;
		if ((Index < 0) || (Index >= (long)m_arOffsets.GetCount()))
			return S_FALSE;
		*pVal = m_arOffsets[Index];
		return S_OK;
	}
	
	STDMETHOD(AddPacket)(long Offset)
	{
		m_arOffsets.Add(Offset);
		return S_OK;
	}
	STDMETHOD(InsertPacket)(long Index, long Offset)
	{
		m_arOffsets.InsertAt(Index, Offset);
		return S_OK;
	}
	STDMETHOD(RemovePacket)(long Index)
	{
		if ((Index < 0) || (Index >= (long)m_arOffsets.GetCount()))
			return S_FALSE;
		m_arOffsets.RemoveAt(Index);
		return S_OK;
	}
	STDMETHOD(RemovePacketAll)()
	{
		m_arOffsets.RemoveAll();
		return S_OK;
	}
protected:
	CAtlArray<long> m_arOffsets;
};
	
[coclass, uuid("43191873-1C10-43ba-B13D-0B827F2CF116"), threading(apartment), vi_progid("AVSMediaCore3.AVSPacketsAudioData"), progid("AVSMediaCore3.AVSPacketsAudioData.1"), version(1.0)]
class ATL_NO_VTABLE CAVSPacketsAudioData : public CAVSPacketsAudioDataImpl<CAVSPacketsAudioData, IAVSPacketsAudioData>
{
};

