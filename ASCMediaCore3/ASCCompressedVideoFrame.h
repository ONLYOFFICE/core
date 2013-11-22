#pragma once

#include "ASCMediaData.h"

[object, uuid("D1AB8643-9E1E-4037-A4C1-D166CCF47467"), dual, pointer_default(unique)]
__interface IAVSCompressedVideoFrame : IAVSMediaData
{
	[id(203000 + 1), propget] HRESULT fccHandler([out, retval] ULONG* pVal);
	[id(203000 + 1), propput] HRESULT fccHandler([in] ULONG newVal);

	[id(203000 + 2), propget] HRESULT IsKeyFrame([out, retval] VARIANT_BOOL* pVal);
	[id(203000 + 3), propget] HRESULT FrameType([out, retval] long* pVal);
	[id(203000 + 3), propput] HRESULT FrameType([in] long newVal);
};
	
template <class CoT, class T>
class CAVSCompressedVideoFrameImpl : public CAVSMediaDataImpl<CoT, T>
{
protected:
	
	ULONG m_lFccHandler;
	LONG m_lFrameType;
	
public:
	
	CAVSCompressedVideoFrameImpl()
	{
		m_lMediaType = MEDIAFORMAT_VIDEO_COMPRESSED;
		m_lFccHandler = 0xFFFFFFFF;
		m_lFrameType = 0;
	}
	
	STDMETHOD(CreateDuplicate)(long DublicateType, IAVSMediaData** pMediaData)
	{
		CAVSMediaDataImpl<CoT, T>::CreateDuplicate(DublicateType, pMediaData);
		if (NULL == *pMediaData)
			return S_OK;

		IAVSCompressedVideoFrame* pAVSVideoData = NULL;
		(*pMediaData)->QueryInterface(__uuidof(IAVSCompressedVideoFrame), (void**)(&pAVSVideoData));
		if (NULL == pAVSVideoData)
		{
			(*pMediaData)->Release();
			*pMediaData = NULL;
			return S_OK;
		}

		pAVSVideoData->put_fccHandler(m_lFccHandler);
		pAVSVideoData->put_FrameType(m_lFrameType);
		RELEASEINTERFACE(pAVSVideoData)

		return S_OK;
	}
	
	STDMETHOD(get_fccHandler)(ULONG* pVal)
	{
		*pVal = m_lFccHandler;

		return S_OK;
	}
	STDMETHOD(put_fccHandler)(ULONG newVal)
	{
		m_lFccHandler = newVal;

		return S_OK;
	}
	STDMETHOD(get_IsKeyFrame)(VARIANT_BOOL* pVal)
	{
		if (m_lFrameType == 1)
			*pVal = VARIANT_TRUE;
		else
			*pVal = VARIANT_FALSE;

		return S_OK;
	}
	STDMETHOD(get_FrameType)(LONG* pVal)
	{
		*pVal = m_lFrameType;

		return S_OK;
	}
	STDMETHOD(put_FrameType)(LONG newVal)
	{
		m_lFrameType = newVal;

		return S_OK;
	}
};
	
[coclass, uuid("B1775B10-FD1E-448d-A5AB-68F1BCDDF4EB"), threading(apartment), vi_progid("AVSMediaCore3.AVSVideoFrameCmp"), progid("AVSMediaCore3.AVSVideoFrameCmp.1"), version(1.0)]
class ATL_NO_VTABLE CAVSCompressedVideoFrame : public CAVSCompressedVideoFrameImpl<CAVSCompressedVideoFrame, IAVSCompressedVideoFrame>
{
};

